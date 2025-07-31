#include "metrics_network.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

metrics_network::metrics_network(std::shared_ptr<prometheus::Registry> registry) {
    _interface_name = detect_interface();
    if (_interface_name.empty()) {
        std::cerr << "[metrics_network] No valid interface found\n";
        return;
    }

    auto& rx_family = prometheus::BuildGauge()
        .Name("network_receive_bytes_total")
        .Help("Total received bytes on network interface")
        .Register(*registry);
    _rx_bytes_gauge = &rx_family.Add({{"interface", _interface_name}});

    auto& tx_family = prometheus::BuildGauge()
        .Name("network_transmit_bytes_total")
        .Help("Total transmitted bytes on network interface")
        .Register(*registry);
    _tx_bytes_gauge = &tx_family.Add({{"interface", _interface_name}});

    std::cout << "[metrics_network] Auto-detected interface: " << _interface_name << std::endl;
}

metrics_network::~metrics_network() {
    _rx_bytes_gauge = nullptr;
    _tx_bytes_gauge = nullptr;
}

std::string metrics_network::detect_interface() {
    std::ifstream netdev("/proc/net/dev");
    if (!netdev.is_open()) return "";

    std::string line;
    std::getline(netdev, line); // header
    std::getline(netdev, line); // header

    while (std::getline(netdev, line)) {
        std::istringstream iss(line);
        std::string iface;
        std::getline(iss, iface, ':');
        iface.erase(0, iface.find_first_not_of(" \t"));
        iface.erase(iface.find_last_not_of(" \t") + 1);
        // loopback
        if (iface != "lo") {
            return iface;
        }
    }
    return "";
}

void metrics_network::update() {
    auto read_net_bytes = [this](bool rx) -> double {
        std::ifstream netdev("/proc/net/dev");
        if (!netdev.is_open()) {
            return 0.0;
        }

        std::string line;
        while (std::getline(netdev, line)) {
            if (line.find(_interface_name + ":") != std::string::npos) {
                std::istringstream iss(line);
                std::string iface;
                iss >> iface;
                if (iface.back() == ':') {
                    iface.pop_back();
                }
                if (iface == _interface_name) {
                    double rx_bytes = 0, tx_bytes = 0;
                    // received bytes (field 1 after iface)
                    iss >> rx_bytes;
                    // skip next 7 fields (packets, errs, drop, fifo, frame, compressed, multicast)
                    for (int i = 0; i < 7; ++i) iss >> std::ws;
                    // transmitted bytes (field 9 after iface)
                    iss >> tx_bytes;
                    return rx ? rx_bytes : tx_bytes;
                }
            }
        }
    };

    try {
        // RX
        double rx = read_net_bytes(true);
        // TX
        double tx = read_net_bytes(false);

        _rx_bytes_gauge->Set(rx);
        _tx_bytes_gauge->Set(tx);

        // unit cast
        auto format_bytes = [](double bytes) -> std::string {
            static const char* units[] = {"B", "KB", "MB", "GB", "TB"};
            int idx = 0;
            while (bytes >= 1024 && idx < 4) {
                bytes /= 1024;
                ++idx;
            }
            std::ostringstream oss;
            oss.imbue(std::locale(""));
            oss << std::fixed << std::setprecision(2) << bytes << " " << units[idx];
            return oss.str();
        };

        std::cout << "[metrics_network] Interface: " << _interface_name
                  << ", RX: " << format_bytes(rx)
                  << ", TX: " << format_bytes(tx) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "[metrics_network] Exception in update: " << e.what() << std::endl;
    }
}