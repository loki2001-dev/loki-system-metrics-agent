#!/bin/bash

# apt update
sudo apt update

# install required package
sudo apt install -y software-properties-common

# add Grafana APT repository
sudo add-apt-repository "deb https://packages.grafana.com/oss/deb stable main"

# add Grafana GPG key
wget -q -O - https://packages.grafana.com/gpg.key | sudo apt-key add -

# apt update
sudo apt update

# install
sudo apt install -y grafana