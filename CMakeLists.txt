cmake_minimum_required(VERSION 3.16.3)
project(ex2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex2
        anomaly_detection_util.cpp
        anomaly_detection_util.h
        AnomalyDetector.h
        MainTrain.cpp
        SimpleAnomalyDetector.cpp
        SimpleAnomalyDetector.h
        timeseries.cpp
        timeseries.h minCircle.cpp minCircle.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h)
