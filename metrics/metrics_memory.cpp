#include "metrics_memory.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace prometheus;

metrics_memory::metrics_memory(std::shared_ptr<Registry> registry) {
    // total
    auto& total_family = BuildGauge()
        .Name("system_memory_total_bytes")
        .Help("Total system memory in bytes")
        .Register(*registry);
    _total_memory_gauge = &total_family.Add({});

    // free
    auto& free_family = BuildGauge()
        .Name("system_memory_free_bytes")
        .Help("Free system memory in bytes")
        .Register(*registry);
    _free_memory_gauge = &free_family.Add({});

    // available
    auto& available_family = BuildGauge()
        .Name("system_memory_available_bytes")
        .Help("Available system memory in bytes")
        .Register(*registry);
    _available_memory_gauge = &available_family.Add({});
}

metrics_memory::~metrics_memory() {
    _total_memory_gauge = nullptr;
}

void metrics_memory::update() {
    const auto& read_value_kb = [](const std::string& key) -> double {
        std::ifstream meminfo("/proc/meminfo");
        if (!meminfo.is_open()) {
            std::cerr << "[metrics_memory] Failed to open /proc/meminfo" << std::endl;
            return 0;
        }

        std::string line;
        while (std::getline(meminfo, line)) {
            std::istringstream iss(line);
            std::string name;
            double value;
            std::string unit;
            if (iss >> name >> value >> unit) {
                if (name.back() == ':') {
                    name.pop_back();
                }
                if (name == key) {
                    return value;
                }
            }
        }

        std::cerr << "[metrics_memory] Key not found: " << key << std::endl;
        return 0;
    };

    try {
        const auto& total = read_value_kb("MemTotal");
        const auto& free = read_value_kb("MemFree");
        const auto& available = read_value_kb("MemAvailable");

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "[metrics_memory] MemTotal: " << (total / 1024.0) << " MB" << std::endl;
        std::cout << "[metrics_memory] MemFree: " << (free / 1024.0) << " MB" << std::endl;
        std::cout << "[metrics_memory] MemAvailable: " << (available / 1024.0) << " MB" << std::endl;

        _total_memory_gauge->Set(total * 1024.0);
        _free_memory_gauge->Set(free * 1024.0);
        _available_memory_gauge->Set(available * 1024.0);
    } catch (const std::exception& e) {
        std::cerr << "[metrics_memory] Exception: " << e.what() << std::endl;
    }
}