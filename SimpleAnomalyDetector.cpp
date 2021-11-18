//
// Created by dor on 10/14/21.
//Dor Huri 209409218
//Aviya Hadad 314802075
//

#include "string.h"
#include "timeseries.h"
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->threshold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    float temp, max = 0;
    string maxCorName;
    map<string, vector<float>>::iterator iter;

    map<string, vector<float >>::iterator itr1, itr2;
    for (int i = 0; i < ts.getTable().size(); i++) {
        for (int j = i + 1; j < ts.getTable().size(); j++) {
            vector<float> v1 = ts.getTable().find(ts.getFeatures().at(i))->second;
            vector<float> v2 = ts.getTable().find(ts.getFeatures().at(j))->second;

            temp = abs(pearson(&(v1).at(0), &(v2).at(0), (int) ts.getTable().begin()->second.size()));

            if (temp > max) {
                max = temp;
                maxCorName = ts.getFeatures().at(j);
            }
        }
        correlatedFeatures corrFeatures;
        corrFeatures.feature1 = ts.getFeatures().at(i);
        corrFeatures.feature2 = maxCorName;
        corrFeatures.corrlation = max;
        Point **arrayOfPoints = new Point *[ts.getRowSize()];
        for (int k = 0; k < ts.getTable().find(maxCorName)->second.size(); k++) {
            arrayOfPoints[k] = new Point(ts.getTable().find(corrFeatures.feature1)->second.at(k),
                                         ts.getTable().find(corrFeatures.feature2)->second.at(k));
        }
        corrFeatures.lin_reg = linear_reg(arrayOfPoints, ts.getTable().find(maxCorName)->second.size());
        float temp, maxDev = 0;
        for (int t = 0; t < ts.getRowSize(); t++) {
            temp = dev(*arrayOfPoints[t], corrFeatures.lin_reg);
            if (temp > maxDev) {
                maxDev = temp;
            }
        }
        corrFeatures.threshold = (float) (maxDev * 1.1);
        if (max >= this->threshold) {
            this->corrFeatures.push_back(corrFeatures);
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    //we get the vectore of the structs and chrck what columns are corr, and according to the data we check if
    Point **newArrayOfPoints = new Point *[ts.getRowSize()];
    vector <AnomalyReport> anomalReportVector;
    for(int i=0; i<ts.getRowSize();i++) {
        vector<float> v1 = ts.getTable().find(this->corrFeatures.at(i).feature1)->second;
        vector<float> v2 = ts.getTable().find(this->corrFeatures.at(i).feature2)->second;
        for (int k = 0; k < ts.getRowSize(); k++) {
            newArrayOfPoints[k] = new Point(v1.at(k),v2.at(k));
        }
        for(int t=0;t<ts.getRowSize();t++) {
            if(dev(*newArrayOfPoints[t], newArrayOfPoints, ts.getRowSize())>this->threshold);{
                string descrip1=this->corrFeatures.at(i).feature1;
                string descrip2=this->corrFeatures.at(i).feature2;
                AnomalyReport *anomalyReport = new AnomalyReport(descrip1+"-"+descrip2,t+1);
                anomalReportVector.push_back(*anomalyReport);
            }
        }
    }
    return anomalReportVector;
}

vector<correlatedFeatures> SimpleAnomalyDetector::getNormalModel() {
    return this->corrFeatures;
}


#include "SimpleAnomalyDetector.h"
