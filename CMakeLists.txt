cmake_minimum_required(VERSION 3.21.1)
project(ex5)

set(CMAKE_CXX_STANDARD 11)

add_executable(ex5 CLI.h CLI.cpp commands.h MainTrain.cpp HybridAnomalyDetector.h HybridAnomalyDetector.cpp SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h)