# system-metrics-visualization-agent (Modern C++ & Prometheus)
- A lightweight and extensible system health monitoring agent written in modern C++.
- Built with [prometheus-cpp](https://github.com/jupp0r/prometheus-cpp), it exposes essential system metrics  
  such as CPU usage, memory usage, and disk I/O through a Prometheus-compatible HTTP endpoint.

---

## Features
- Real-time system metrics collection (CPU, Memory, Disk, Network)
- Prometheus exporter endpoint (default: `:9500/metrics`)
- Grafana-ready visualization support

---

## Getting Started
### Prerequisites
- Linux (Ubuntu 20.04 or later recommended)
- Requires CMake 3.14 or later
- Requires C++17 or later compiler
- [prometheus-cpp](https://github.com/jupp0r/prometheus-cpp) (included as a submodule)

---

## Build Instructions
### Setup and Installation
```bash
# Update package lists
sudo apt update

# Install dependencies (libcurl required for prometheus-cpp)
sudo apt install -y libcurl4-openssl-dev

# Clone the repository
git clone https://github.com/loki2001-dev/system-metrics-visualization-agent.git
cd system-metrics-visualization-agent

# Initialize submodule
git submodule update --init --recursive

# Install Grafana (script included in the project)
. install_grafana.sh

# Install Prometheus (script included in the project)
. install_prometheus.sh

# Start Prometheus and Grafana services
. run_prometheus_grafana.sh

# Build the project
. build_project.sh