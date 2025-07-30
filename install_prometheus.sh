#!/bin/bash

# Change directory
cd /opt

# Download Prometheus
sudo wget https://github.com/prometheus/prometheus/releases/download/v2.52.0/prometheus-2.52.0.linux-amd64.tar.gz

# Extract
sudo tar -xvf prometheus-2.52.0.linux-amd64.tar.gz

# Rename
sudo mv prometheus-2.52.0.linux-amd64 prometheus

# Change
cd prometheus