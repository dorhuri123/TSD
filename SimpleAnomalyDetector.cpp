//
//Dor Huri 209409218
//Aviya Hadad 314802075
//

#include <cmath>
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->topThreshold = 0.9;
    this->bottomThreshold = 0.5;
}


SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

//identify if there is correlation and add struct to the vector
void SimpleAnomalyDetector::apply(float corr, const correlatedFeatures &cf, Point **points, int size) {
    if (corr >= this->topThreshold) {
        //adding struct to vector
        this->corrFeatures.push_back(cf);
    }
}

//learning at the offline stage bout the correlation it should be
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    {
        float temp, max = 0;
        string maxCorName;
        //going through vector map for looping
        map < string, vector < float >> ::iterator
        iter;
        map < string, vector < float >> ::iterator
        itr1, itr2;
        //for looping trow column of map
        for (int i = 0; i < ts.getTable().size(); ++i) {
            //also looping trow column of map
            for (int j = i + 1; j < ts.getTable().size(); ++j) {
                //assigning the vector of keys
                vector<float> v1 = ts.getTable().find(ts.getFeatures().at(i))->second;
                vector<float> v2 = ts.getTable().find(ts.getFeatures().at(j))->second;
                //calculating the correlation between vectors
                temp = fabs(pearson(&(v1).at(0), &(v2).at(0), (int) ts.getTable().begin()->second.size()));
                //checking if the current correlation is  more correlative than the last correlative value
                if (temp > max) {
                    //max get the highest correlation
                    max = temp;
                    //assigning the name of column of the vector with the highest correlation
                    maxCorName = ts.getFeatures().at(j);
                }
            }
            //initializing the struct
            correlatedFeatures cf = {};
            //getting the vectors name
            cf.feature1 = ts.getFeatures().at(i);
            cf.feature2 = maxCorName;
            //the value of the highest correlation
            cf.corrlation = max;
            //pointer array for liner regression
            Point **arrayOfPoints = new Point *[ts.getRowSize()];
            //looping trow the vectors
            for (int k = 0; k < ts.getRowSize(); k++) {
                //assigning the point according to the matching vectors value
                arrayOfPoints[k] = new Point(ts.getTable().find(cf.feature1)->second.at(k),
                                             ts.getTable().find(cf.feature2)->second.at(k));
            }
            //create the liner regression from points array
            cf.lin_reg = linear_reg(arrayOfPoints, ts.getTable().find(maxCorName)->second.size());
            float temp, maxDev = 0;
            //looping trow array point

            for (int t = 0; t < ts.getRowSize(); t++) {
                //getting the distance point of liner regression
                temp = dev(*arrayOfPoints[t], cf.lin_reg);
                //checking if current distance is greater than max dev
                if (temp > maxDev) {
                    maxDev = temp;
                }
            }
            // apply()
            //multiplying topThreshold by const 1.1 for avoiding close error
            cf.threshold = (float) (maxDev * 1.1);
            //if correlation is relevant for 0.9
            apply(max, cf, arrayOfPoints, ts.getRowSize());
            max = 0;
        }

    }
}

//call for another func to identify and create report and add it to the vector
void SimpleAnomalyDetector::addReport(vector <AnomalyReport> &anomalReportVector, int index,
                                      int size, vector<float> v1, vector<float> v2, Point **newArrayOfPoints) {
    simpleAnomalyReport(anomalReportVector, index, size, v1, v2, newArrayOfPoints);

}

vector <AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    Point **newArrayOfPoints = new Point *[ts.getRowSize()];
    //creating vector of AnomalyReport
    vector <AnomalyReport> anomalReportVector;
    for (int i = 0; i < this->corrFeatures.size(); i++) {
        //take 2 vectors from struct
        vector<float> v1 = ts.getTable().find(this->corrFeatures.at(i).feature1)->second;
        vector<float> v2 = ts.getTable().find(this->corrFeatures.at(i).feature2)->second;
        //looping through the currents vectors
        for (int k = 0; k < ts.getRowSize(); k++) {
            //creating point array
            newArrayOfPoints[k] = new Point(v1.at(k), v2.at(k));
        }
        //sending the correlative vectors to check if we have anormaly
        addReport(anomalReportVector, i, ts.getRowSize(), v1, v2, newArrayOfPoints);
    }
    //returning vector
    return anomalReportVector;
}


vector <correlatedFeatures> SimpleAnomalyDetector::getNormalModel() {
    //return the vector of struct corrfeatures(offline)
    return this->corrFeatures;
}

//detect if we have anormlay according to the data and add report to the vector
void SimpleAnomalyDetector::simpleAnomalyReport(vector <AnomalyReport> &anomalReportVector, int index, int size,
                                                vector<float> v1, vector<float> v2, Point **newArrayOfPoints) {
    //looping trow the array
    for (int t = 0; t < size; t++) {
        //checking if we have deviation according to the topThreshold
        if (dev(*newArrayOfPoints[t], newArrayOfPoints, size) >
            this->corrFeatures.at(index).threshold) {
            //assigning name of vector
            string descrip1 = this->corrFeatures.at(index).feature1;
            string descrip2 = this->corrFeatures.at(index).feature2;

            //creating instace of annomaly report
            AnomalyReport *anomalyReport = new AnomalyReport(descrip1 + "-" + descrip2, t + 1);
            //adding anomaly report to the vector
            anomalReportVector.push_back(*anomalyReport);
        }
    }
}
