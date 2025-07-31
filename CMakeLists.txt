CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT(system-metrics-visualization-agent)

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
ADD_SUBDIRECTORY(3rdparty/spdlog)

# INCLUDE
INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}
        3rdparty/prometheus-cpp/core/include
        3rdparty/prometheus-cpp/pull/include
        3rdparty/spdlog/include
)

# GLOB
FILE(GLOB_RECURSE METRICS_SOURCES
        "${PROJECT_SOURCE_DIR}/metrics/*.cpp"
        "${PROJECT_SOURCE_DIR}/metrics/*.h"
)

# DEFINE executable files
ADD_EXECUTABLE(system-metrics-visualization-agent
        main.cpp
        ${METRICS_SOURCES}
)

# INCLUDE directories (OPTIONAL)
TARGET_INCLUDE_DIRECTORIES(system-metrics-visualization-agent PRIVATE
        ${PROJECT_SOURCE_DIR}
        3rdparty/prometheus-cpp/core/include
        3rdparty/prometheus-cpp/pull/include
        3rdparty/spdlog/include
)

# LINK
TARGET_LINK_LIBRARIES(system-metrics-visualization-agent
        prometheus-cpp::core
        prometheus-cpp::pull
)