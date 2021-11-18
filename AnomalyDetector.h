//
// Created by dor on 10/14/21.
//Dor Huri 209409218
//Aviya Hadad 314802075
//


#ifndef AVIA1_ANOMALYDETECTOR_H
#define AVIA1_ANOMALYDETECTOR_H

#include "timeseries.h"

using namespace std;

class AnomalyReport{
public:
    const string description;
    const long timeStep;
    AnomalyReport(string description, long timeStep) :
            description(description),timeStep(timeStep){}
};
class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector(){}
};

#endif //AVIA1_ANOMALYDETECTOR_H
