//
//Dor Huri 209409218
//Aviya Hadad 314802075
//

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {

public:
    HybridAnomalyDetector();

    virtual ~HybridAnomalyDetector();

    //void learnNormal(const TimeSeries &ts);
    void apply(float corr, const correlatedFeatures &cf, Point **points, int size);

    void addReport(vector <AnomalyReport> &anomalReportVector, int index,
                   int size, vector<float> v1, vector<float> v2, Point **newArrayOfPoints);

    virtual correlatedFeatures addDataOfCircle(Point **arrayOfPoints, int size, correlatedFeatures corrFeatures);

    virtual void hybridAnomalyReport(vector <AnomalyReport> &anomalReportVector, int index, int size, vector<float> v1,
                                     vector<float> v2,
                                     Point **newArrayOfPoints);
    HybridAnomalyDetector(float threshold);
    void setCorllation(float corllation);
};
#endif /* HYBRIDANOMALYDETECTOR_H_ */
