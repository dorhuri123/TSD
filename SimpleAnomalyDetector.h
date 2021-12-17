//
//Dor Huri 209409218
//Aviya Hadad 314802075
//


#ifndef AVIA1_SIMPLEANOMALYDETECTOR_H
#define AVIA1_SIMPLEANOMALYDETECTOR_H

#include "strings.h"
#include "anomaly_detection_util.h"
//#include "timeseries.h"
#include "AnomalyDetector.h"
#include <algorithm>

struct correlatedFeatures {
    //the vectors that are corrlation
    string feature1, feature2; // names of the correlated features
    //the value of the corrlation
    float corrlation;
    //the regression line of the vector
    Line lin_reg;
    //topThreshold value.
    float threshold;
    //the center of the circle.
    Point center;
    //the radius of the circle.
    float radius;
    //is it a circle.
    bool isCircle = false;
};


class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
protected:
    float topThreshold;
    float bottomThreshold;
    // we want to save cf data and anomaly report.
    vector <correlatedFeatures> corrFeatures;
public:
    SimpleAnomalyDetector();

    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries &ts);

    virtual void apply(float corr, const correlatedFeatures &cf, Point **points, int size);

    virtual void addReport(vector <AnomalyReport> &anomalReportVector, int index,
                           int size, vector<float> v1, vector<float> v2, Point **newArrayOfPoints);

    virtual vector <AnomalyReport> detect(const TimeSeries &ts);

    vector <correlatedFeatures> getNormalModel();

    virtual void simpleAnomalyReport(vector <AnomalyReport> &anomalReportVector, int index, int size, vector<float> v1,
                                     vector<float> v2,
                                     Point **newArrayOfPoints);
};


#endif //AVIA1_SIMPLEANOMALYDETECTOR_H
