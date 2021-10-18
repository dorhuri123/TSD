cmake_minimum_required(VERSION 3.20)
project(TSD)

set(CMAKE_CXX_STANDARD 14)

add_executable(TSD main.cpp anomaly_detection_util.cpp anomaly_detection_util.h)

add_executable(anomaly_detection_util anomaly_detection_util.cpp)