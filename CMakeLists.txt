CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT(loki-system-metrics-agent)

# OPTION (Standard)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# OPTION (3rdparty)
SET(PROMETHEUS_CPP_PULL_EXPORTER ON)
SET(PROMETHEUS_CPP_PUSH_EXPORTER OFF)
SET(BUILD_SHARED_LIBS ON)
SET(BUILD_TESTING OFF)
SET(BUILD_PROMETHEUS_CPP_TESTS OFF)
SET(BUILD_PROMETHEUS_CPP_EXAMPLES OFF)

# ADD 3rdparty
ADD_SUBDIRECTORY(3rdparty/prometheus-cpp)

# INCLUDE
INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}
        3rdparty/prometheus-cpp/core/include
        3rdparty/prometheus-cpp/pull/include
)

# DEFINE executable files
ADD_EXECUTABLE(loki-system-metrics-agent
        main.cpp
)

# LINK
TARGET_LINK_LIBRARIES(loki-system-metrics-agent
        prometheus-cpp::core
        prometheus-cpp::pull
)