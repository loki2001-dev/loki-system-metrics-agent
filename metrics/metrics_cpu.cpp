#include "metrics_cpu.h"
#include <fstream>
#include <string>
#include <spdlog/spdlog.h>

metrics_cpu::metrics_cpu(const std::shared_ptr<prometheus::Registry>& registry)
    : _prevTotal(0), _prevIdle(0) {
    auto& family = prometheus::BuildGauge()
        .Name("system_cpu_usage")
        .Help("CPU usage in percentage")
        .Register(*registry);

    _cpu_usage_gauge = &family.Add({{"core", "total"}});
}

metrics_cpu::~metrics_cpu() {
}

void metrics_cpu::update() {
    auto read_cpu_usage = [this]() -> double {
        std::ifstream file("/proc/stat");
        if (!file.is_open()) {
            spdlog::error("[metrics_cpu] Failed to open /proc/stat");
            return 0.0;
        }

        std::string cpu;
        long user, nice, system, idle, iowait, irq, softirq;
        file >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq;

        long totalIdle = idle + iowait;
        long totalNonIdle = user + nice + system + irq + softirq;
        long total = totalIdle + totalNonIdle;

        long deltaTotal = total - _prevTotal;
        long deltaIdle = totalIdle - _prevIdle;

        _prevTotal = total;
        _prevIdle = totalIdle;

        if (deltaTotal == 0) return 0.0;
        return (1.0 - static_cast<double>(deltaIdle) / deltaTotal) * 100.0;
    };

    const double usage = read_cpu_usage();
    _cpu_usage_gauge->Set(usage);

    spdlog::info("[metrics_cpu] CPU Usage: {:.2f} %", usage);
}