# loki-system-metrics-agent (Modern C++ & Prometheus)
- A lightweight and extensible system health monitoring agent written in modern C++.
- Built with [prometheus-cpp](https://github.com/jupp0r/prometheus-cpp), it exposes essential system metrics  
  such as CPU usage, memory usage, and disk I/O through a Prometheus-compatible HTTP endpoint.

---

## Features
- Real-time system metrics collection (CPU, memory, uptime, etc.)
- Prometheus exporter endpoint (default: `:8080/metrics`)
- Grafana-ready visualization support

---

## Getting Started
### Prerequisites
- Requires CMake 3.14 or later
- Requires C++17 or later compiler
- [prometheus-cpp](https://github.com/jupp0r/prometheus-cpp) (included as a submodule)

---

## Build Instructions
### Example
```bash
sudo apt update

# Install the libcurl4-openssl-dev package(for. prometheus-cpp).
sudo apt install libcurl4-openssl-dev

# Clone the repository
git clone https://github.com/loki2001-dev/loki-system-metrics-agent.git
cd sys-metrics-agent

# Initialize submodule
git submodule update --init --recursive

# Build the project
rm -rf build
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .