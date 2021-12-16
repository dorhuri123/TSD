//
//Dor Huri 209409218
//Aviya Hadad 314802075
//

#include <cmath>
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->topThreshold = 0.9;
    this->bottomThreshold=0.5;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {

}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {

}
vector<correlatedFeatures> SimpleAnomalyDetector::getNormalModel() {
    //return the vector of struct corrfeatures(offline)
    return this->corrFeatures;
}
