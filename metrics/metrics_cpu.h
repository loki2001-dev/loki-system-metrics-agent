#ifndef METRICS_CPU_H
#define METRICS_CPU_H

#pragma once

#include <memory>
#include <prometheus/gauge.h>
#include <prometheus/registry.h>

class metrics_cpu : public std::enable_shared_from_this<metrics_cpu>{
public:
    explicit metrics_cpu(std::shared_ptr<prometheus::Registry> registry);
    virtual ~metrics_cpu();

    void update();

private:
    double read_cpu_usage();

    long _prevTotal;
    long _prevIdle;

    prometheus::Gauge* _cpu_usage_gauge;
};

#endif // METRICS_CPU_H