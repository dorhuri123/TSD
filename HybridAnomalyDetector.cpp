
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}



correlatedFeatures HybridAnomalyDetector::addDataOfCircle(Point **arrayOfPoints,int size,correlatedFeatures corrFeatures){
    Circle minCircle=findMinCircle(arrayOfPoints,size);
    corrFeatures.radius= minCircle.radius;
    corrFeatures.center= minCircle.center;
    corrFeatures.isCircle= true;
    return corrFeatures;
}
void HybridAnomalyDetector::hybridAnomalyReport(vector <AnomalyReport> anomalReportVector,
                                            correlatedFeatures corrFeatures,int size, vector <float> v1,vector <float> v2,Point **newArrayOfPoints) {
for(int i=0;i<size;i++){
    if(inside_circle(new Circle(corrFeatures.center,corrFeatures.radius),newArrayOfPoints[i])==false){
        string descrip1 = corrFeatures.feature1;
        string descrip2 = corrFeatures.feature2;
        AnomalyReport *anomalyReport = new AnomalyReport(descrip1+"-"+descrip2,i+1);
        anomalReportVector.push_back(*anomalyReport);
    }

}
}


        HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

