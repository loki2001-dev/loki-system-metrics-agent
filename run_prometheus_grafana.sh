#!/bin/bash

PROMETHEUS_DIR="/opt/prometheus"
PROMETHEUS_BIN="$PROMETHEUS_DIR/prometheus"
PROMETHEUS_YML="$PROMETHEUS_DIR/prometheus.yml"
PROMETHEUS_LOG="$PROMETHEUS_DIR/prometheus.log"

# Grafana
GRAFANA_BIN="/usr/sbin/grafana-server"
#GRAFANA_LOG="/var/log/grafana/grafana.log"
GRAFANA_LOG="$HOME/grafana.log"

echo "[Starting Prometheus...]"
nohup "$PROMETHEUS_BIN" --config.file="$PROMETHEUS_YML" > "$PROMETHEUS_LOG" 2>&1 &

PROM_PID=$!
echo "Prometheus PID: $PROM_PID"
echo "Prometheus log: $PROMETHEUS_LOG"

echo "================================================"

if [ -x "$GRAFANA_BIN" ]; then
  echo "[Starting Grafana...]"
  nohup "$GRAFANA_BIN" --homepath=/usr/share/grafana > "$GRAFANA_LOG" 2>&1 &
  GRAFANA_PID=$!
  echo "Grafana PID: $GRAFANA_PID"
  echo "Grafana LOG: $GRAFANA_LOG"
else
  echo "Grafana executable not found at $GRAFANA_BIN."
fi

echo "================================================"

echo "All services have been started in the background."