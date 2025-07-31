#include "metrics_disk.h"
#include <sys/statvfs.h>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <spdlog/spdlog.h>

metrics_disk::metrics_disk(std::shared_ptr<prometheus::Registry> registry, const std::string& path)
    : _path(path) {
    auto& total_family = prometheus::BuildGauge()
        .Name("system_disk_total_bytes")
        .Help("Total disk space in bytes")
        .Register(*registry);
    _disk_total_gauge = &total_family.Add({});

    auto& available_family = prometheus::BuildGauge()
        .Name("system_disk_available_bytes")
        .Help("Available disk space in bytes")
        .Register(*registry);
    _disk_available_gauge = &available_family.Add({});
}

metrics_disk::~metrics_disk() {
    _disk_total_gauge = nullptr;
    _disk_available_gauge = nullptr;
}

void metrics_disk::update() {
    // statvfs
    auto get_disk_bytes = [this](auto stat_func) -> double {
        struct statvfs stat{};
        if (statvfs(_path.c_str(), &stat) != 0) {
            throw std::runtime_error("Failed to get disk stats for path: " + _path);
        }
        return stat_func(stat) * static_cast<double>(stat.f_frsize);
    };

    // unit cast
    auto format_bytes = [](double bytes) -> std::string {
        static const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
        int unit_index = 0;
        while (bytes >= 1024 && unit_index < 5) {
            bytes /= 1024;
            ++unit_index;
        }
        std::ostringstream oss;
        oss.imbue(std::locale(""));
        oss << std::fixed << std::setprecision(2) << bytes << " " << units[unit_index];
        return oss.str();
    };

    try {
        const double total = get_disk_bytes([](const struct statvfs& s) { return static_cast<double>(s.f_blocks); });
        const double available = get_disk_bytes([](const struct statvfs& s) { return static_cast<double>(s.f_bavail); });

        _disk_total_gauge->Set(total);
        _disk_available_gauge->Set(available);

        spdlog::info("[metrics_disk] Disk Total: {}, Available: {}", format_bytes(total), format_bytes(available));
    } catch (const std::exception& e) {
        spdlog::error("[metrics_disk] Exception in update: {}", e.what());
    }
}