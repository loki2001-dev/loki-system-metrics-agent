#include "metrics_memory.h"
#include <fstream>
#include <sstream>
#include <string>
#include <spdlog/spdlog.h>

using namespace prometheus;

metrics_memory::metrics_memory(const std::shared_ptr<Registry>& registry) {
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
            spdlog::error("[metrics_memory] Failed to open /proc/meminfo");
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

        spdlog::error("[metrics_memory] Key not found: {}", key);
        return 0;
    };

    try {
        const auto& total = read_value_kb("MemTotal");
        const auto& free = read_value_kb("MemFree");
        const auto& available = read_value_kb("MemAvailable");

        spdlog::info("[metrics_memory] MemTotal: {:.2f} MB", total / 1024.0);
        spdlog::info("[metrics_memory] MemFree: {:.2f} MB", free / 1024.0);
        spdlog::info("[metrics_memory] MemAvailable: {:.2f} MB", available / 1024.0);

        _total_memory_gauge->Set(total * 1024.0);
        _free_memory_gauge->Set(free * 1024.0);
        _available_memory_gauge->Set(available * 1024.0);
    } catch (const std::exception& e) {
        spdlog::error("[metrics_memory] Exception: {}", e.what());
    }
}