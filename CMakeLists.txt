cmake_minimum_required(VERSION 3.16)
project(ex5_Avia)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(ex5_Avia
        MainTrain.cpp
        anomaly_detection_util.cpp
        anomaly_detection_util.h
        AnomalyDetector.h
        SimpleAnomalyDetector.cpp
        SimpleAnomalyDetector.h
        timeseries.cpp
        timeseries.h minCircle.cpp minCircle.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h CLI.h CLI.cpp
        commands.h Server.h Server.cpp)