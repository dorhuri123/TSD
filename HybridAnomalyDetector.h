

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual correlatedFeatures addDataOfCircle(Point **arrayOfPoints,int size,correlatedFeatures corrFeatures);
    virtual void hybridAnomalyReport(vector <AnomalyReport> anomalReportVector,
                            correlatedFeatures corrFeatures,int size, vector <float> v1,vector <float> v2,Point **newArrayOfPoints) {
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
