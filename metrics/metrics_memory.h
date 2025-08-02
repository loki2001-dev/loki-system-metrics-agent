#ifndef METRICS_MEMORY_H
#define METRICS_MEMORY_H

#pragma once

#include <memory>
#include <prometheus/gauge.h>
#include <prometheus/registry.h>

class metrics_memory : public std::enable_shared_from_this<metrics_memory> {
public:
    explicit metrics_memory(const std::shared_ptr<prometheus::Registry>& registry);
    virtual ~metrics_memory();

    void update();

private:
    double read_value_kb(const std::string& key);

    prometheus::Gauge* _total_memory_gauge;
    prometheus::Gauge* _free_memory_gauge;
    prometheus::Gauge* _available_memory_gauge;
};

#endif // METRICS_MEMORY_H