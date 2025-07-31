#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <atomic>
#include <array>
#include <chrono>
#include <csignal>
#include <iostream>
#include <memory>
#include <stdint.h>
#include <thread>

#include "metrics/metrics_cpu.h"
#include "metrics/metrics_memory.h"

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

const std::array<int, 6> handled_signals = {
    SIGINT, SIGTERM, SIGTSTP, SIGHUP, SIGQUIT, SIGUSR1
};

void initialize() {
    // signal
    for (int sig : handled_signals) {
        std::signal(sig, signal_handler);
    }
}

int32_t main(int32_t argc, char *argv[]) {
    initialize();

    const auto registry = std::make_shared<prometheus::Registry>();
    metrics_cpu cpu_monitor(registry);
    metrics_memory memory_monitor(registry);

    prometheus::Exposer exposer{"127.0.0.1:9500"};
    exposer.RegisterCollectable(registry);

    std::thread monitor_thread([&]() {
        while (running) {
            try {
                // usage - cpu
                cpu_monitor.update();
                // usage - memory
                memory_monitor.update();
            } catch (const std::exception& e) {
                std::cerr << "[Error] Exception in update(): " << e.what() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });

    monitor_thread.join();

    return 0;
}