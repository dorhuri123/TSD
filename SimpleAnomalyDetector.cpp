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
    //going trow vector map for looping
    map<string, vector<float>>::iterator iter;
    map<string, vector<float >>::iterator itr1, itr2;
    //for looping trow column of map
    for (int i = 0; i < ts.getTable().size(); ++i) {
        //also looping trow column of map
        for (int j = i + 1; j < ts.getTable().size(); ++j) {
            //assigning the vector of keys
            vector<float> v1 = ts.getTable().find(ts.getFeatures().at(i))->second;
            vector<float> v2 = ts.getTable().find(ts.getFeatures().at(j))->second;
            //calculating the correlation between vectors
            temp = abs(pearson(&(v1).at(0), &(v2).at(0), (int) ts.getTable().begin()->second.size()));
            //checking if the current correlation is  more correlative than the last correlative value
            if (temp > max) {
                //max get the highest correlation
                max = temp;
                //assigning the name of column of the vector with the highest correlation
                maxCorName = ts.getFeatures().at(j);
            }
        }
        //initializing the struct
        correlatedFeatures corrFeatures;
        //getting the vectors name
        corrFeatures.feature1 = ts.getFeatures().at(i);
        corrFeatures.feature2 = maxCorName;
        //the value of the highest correlation
        corrFeatures.corrlation = max;
        //pointer array for liner regression
        Point **arrayOfPoints = new Point *[ts.getRowSize()];
        //looping trow the vectors
        for (int k = 0; k < ts.getRowSize(); k++) {
            //assigning the point according to the matching vectors value
            arrayOfPoints[k] = new Point(ts.getTable().find(corrFeatures.feature1)->second.at(k),
                                         ts.getTable().find(corrFeatures.feature2)->second.at(k));
        }
        //create the liner regression from points array
        corrFeatures.lin_reg = linear_reg(arrayOfPoints, ts.getTable().find(maxCorName)->second.size());
        float temp, maxDev = 0;
        //looping trow array point
        for (int t = 0; t < ts.getRowSize(); t++) {
            //getting the distance point of liner regression
            temp = dev(*arrayOfPoints[t], corrFeatures.lin_reg);
            //checking if current distance is greater than max dev
            if (temp > maxDev) {
                maxDev = temp;
            }
        }
        //multiplying threshold by const 1.1 for avoiding close error
        corrFeatures.threshold = (float) (maxDev * 1.1);
        //if correlation is relevant for 0.9
        if (max >= this->threshold) {
            //adding struct to vector
            this->corrFeatures.push_back(corrFeatures);
        }
        max=0;
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    //we get the vector of the structs and check what columns are corr, and according to the data we check if
    Point **newArrayOfPoints = new Point *[ts.getRowSize()];
    //creating vector of AnomalyReport
    vector <AnomalyReport> anomalReportVector;
    for(int i=0; i<this->corrFeatures.size();i++) {
        //take 2 vectors from struct
        vector<float> v1 = ts.getTable().find(this->corrFeatures.at(i).feature1)->second;
        vector<float> v2 = ts.getTable().find(this->corrFeatures.at(i).feature2)->second;
        //looping trow the currents vectors
        for (int k = 0; k < ts.getRowSize(); k++) {
            //creating point array
            newArrayOfPoints[k] = new Point(v1.at(k),v2.at(k));
        }
        //looping trow the array
        for(int t=0;t<ts.getRowSize();t++) {
            //checking if we have deviation according to the threshold
            if(dev(*newArrayOfPoints[t], newArrayOfPoints, ts.getRowSize())>this->corrFeatures.at(i).threshold) {
                //assigning name of vector
                string descrip1=this->corrFeatures.at(i).feature1;
                string descrip2=this->corrFeatures.at(i).feature2;
                //creating instace of annomaly report
                AnomalyReport *anomalyReport = new AnomalyReport(descrip1+"-"+descrip2,t+1);
                //adding anomaly report to the vector
                anomalReportVector.push_back(*anomalyReport);
            }
        }
    }
    //returning vector
    return anomalReportVector;
}

vector<correlatedFeatures> SimpleAnomalyDetector::getNormalModel() {
    //return the vector of struct corrfeatures(offline)
    return this->corrFeatures;
}


#include "SimpleAnomalyDetector.h"
