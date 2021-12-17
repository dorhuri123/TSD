//
//Dor Huri 209409218
//Aviya Hadad 314802075
//
#include "HybridAnomalyDetector.h"

//constructor
HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

//detect abnormal for circle struct at the online stage and update and return the vector of the anomalyreport
void HybridAnomalyDetector::hybridAnomalyReport(vector <AnomalyReport> &anomalReportVector, int index,
                                                int size, vector<float> v1, vector<float> v2,
                                                Point **newArrayOfPoints) {
//looping through the points of the correlative vector to check if a point is not in the circle
    //Circle circle= new Circle(corrFeatures[i].center,corrFeatures[i].threshold);
    for (int t = 0; t < size; t++) {
        //checking if we have deviation according to the topThreshold
        float dist = distance(corrFeatures[index].center, (*newArrayOfPoints[t]));
        if (dist > corrFeatures[index].threshold) {
            //assigning name of vector
            string descrip1 = corrFeatures[index].feature1;
            string descrip2 = corrFeatures[index].feature2;

            //creating instace of annomaly report
            AnomalyReport *anomalyReport = new AnomalyReport(descrip1 + "-" + descrip2, (t + 1));
            //adding anomaly report to the vector
            anomalReportVector.push_back(*anomalyReport);
        }
    }
}

//identify according to the threshold if it really correlative and if so sort it
// to be circle or line (if it a circle add data to the struct) and update the vector
void HybridAnomalyDetector::apply(float corr, const correlatedFeatures &cf, Point **points, int size) {
    if (corr >= this->topThreshold) {
        //adding struct to vector
        this->corrFeatures.push_back(cf);
    }
    //add data and add the sruct to the vector of structs corrfeatures
    if ((this->topThreshold > corr) && (corr >= this->bottomThreshold)) {
        this->corrFeatures.push_back(addDataOfCircle(points, size, cf));
    }
}

//add Info about the correlation in case it a circle
correlatedFeatures
HybridAnomalyDetector::addDataOfCircle(Point **arrayOfPoints, int size, correlatedFeatures corrFeatures) {
    //create minimal circle according to the correlative vectors
    Circle minCircle = findMinCircle(arrayOfPoints, size);
    //add the radius of the minimal circle to the struct
    corrFeatures.radius = minCircle.radius;
    //same just with center
    corrFeatures.center = (minCircle.center);
    //updating the indicator to know at the online if a struct is of circle or line
    corrFeatures.isCircle = true;
    corrFeatures.threshold = minCircle.radius * 1.1;
    //returning the struct we just now updated
    return corrFeatures;
}

//we know we have correlation and here we indetify if the struct is a circle or line to send it for check if
//there is anormaly in the relevant function
void HybridAnomalyDetector::addReport(vector <AnomalyReport> &anomalReportVector, int index, int size, vector<float> v1,
                                      vector<float> v2, Point **newArrayOfPoints) {
    //at the online stage we check if the struct is of a kind circle
    if (corrFeatures.at(index).isCircle == true) {
        //send it to maybe add report of a kind circle to the vector of the report
        hybridAnomalyReport(anomalReportVector, index, size, v1, v2, newArrayOfPoints);
    } else
        //sent it to maybe add report of a kind line to the vector of the report
    {
        simpleAnomalyReport(anomalReportVector, index, size, v1, v2, newArrayOfPoints);
    }
}





