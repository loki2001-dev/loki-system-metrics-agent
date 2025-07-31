#ifndef METRICS_NETWORK_H
#define METRICS_NETWORK_H

#pragma once

#include <memory>
#include <prometheus/gauge.h>
#include <prometheus/registry.h>
#include <string>

class metrics_network : public std::enable_shared_from_this<metrics_network>{
public:
    explicit metrics_network(std::shared_ptr<prometheus::Registry> registry);
    virtual ~metrics_network();

    void update();

private:
    std::string detect_interface();
    double read_rx_bytes();
    double read_tx_bytes();

    prometheus::Gauge* _rx_bytes_gauge;
    prometheus::Gauge* _tx_bytes_gauge;

    std::string _interface_name;
};

#endif // METRICS_NETWORK_H