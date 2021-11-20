//
//Dor Huri 209409218
//Aviya Hadad 314802075
//


#ifndef AVIA1_SIMPLEANOMALYDETECTOR_H
#define AVIA1_SIMPLEANOMALYDETECTOR_H

#include "strings.h"
#include "anomaly_detection_util.h"
#include "timeseries.h"
#include "AnomalyDetector.h"
#include <algorithm>

struct correlatedFeatures{
    //the vectors that are corrlation
    string feature1,feature2; // names of the correlated features
    //the value of the corrlation
    float corrlation;
    //the regression line of the vector
    Line lin_reg;
    //threshold value
    float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
    float threshold;
    // we want to save cf data and anomaly report.
    vector<correlatedFeatures> corrFeatures;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel();
};


#endif //AVIA1_SIMPLEANOMALYDETECTOR_H
