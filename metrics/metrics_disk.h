#ifndef METRICS_DISK_H
#define METRICS_DISK_H

#pragma once

#include <memory>
#include <prometheus/gauge.h>
#include <prometheus/registry.h>
#include <string>

class metrics_disk : public std::enable_shared_from_this<metrics_disk>{
public:
    explicit metrics_disk(const std::shared_ptr<prometheus::Registry>& registry, std::string  path = "/");
    virtual ~metrics_disk();

    void update();

private:
    prometheus::Gauge* _disk_total_gauge;
    prometheus::Gauge* _disk_available_gauge;
    std::string _path;
};

#endif // METRICS_DISK_H