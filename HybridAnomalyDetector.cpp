
#include "HybridAnomalyDetector.h"
//constructor
HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub
}
HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

//detect abnormal at the online stage and returning the vectore of the report
void HybridAnomalyDetector::hybridAnomalyReport(vector <AnomalyReport> anomalReportVector,
                                                correlatedFeatures corrFeatures,int size, vector <float> v1,vector <float> v2,Point **newArrayOfPoints) {
//looping through the points of the correlative vector to check if a point is not in the circle
    for(int i=0;i<size;i++){
        if(inside_circle(*(new Circle(*(corrFeatures.center),corrFeatures.threshold)),*newArrayOfPoints[i])==false){
            //taking description of the vectors name if we found point not in the circle
            string descrip1 = corrFeatures.feature1;
            string descrip2 = corrFeatures.feature2;
            //initialize anomaly report struct and add the description and the time stamp
            AnomalyReport *anomalyReport = new AnomalyReport(descrip1+"-"+descrip2,i+1);
            //adding the struct to the report vector
            anomalReportVector.push_back(*anomalyReport);
        }

    }
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts){
    float temp, max = 0;
    string maxCorName;
    //going through vector map for looping
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
        //multiplying topThreshold by const 1.1 for avoiding close error
        corrFeatures.threshold = (float) (maxDev * 1.1);
        //if correlation is relevant for 0.9
        if (max >= this->topThreshold) {
            //adding struct to vector
            this->corrFeatures.push_back(corrFeatures);
        }
        if((this->topThreshold>=max)&&(max>= this->bottomThreshold)){
            this->corrFeatures.push_back(addDataOfCircle(arrayOfPoints,ts.getRowSize(),corrFeatures));
        }
        max=0;
    }
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries& ts){
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
        //at the online stage we check if the struct is of a kind circle
        if (corrFeatures.at(i).isCircle == true) {
            //send it to add report of a kind circle to the vector of the report
            hybridAnomalyReport(anomalReportVector, corrFeatures.at(i), ts.getRowSize(),v1,v2, newArrayOfPoints);
        } else {
            //looping trow the array
            for (int t = 0; t < ts.getRowSize(); t++) {
                //checking if we have deviation according to the topThreshold
                if (dev(*newArrayOfPoints[t], newArrayOfPoints, ts.getRowSize()) >
                    this->corrFeatures.at(i).threshold) {
                    //assigning name of vector
                    string descrip1 = this->corrFeatures.at(i).feature1;
                    string descrip2 = this->corrFeatures.at(i).feature2;

                    //creating instace of annomaly report
                    AnomalyReport *anomalyReport = new AnomalyReport(descrip1 + "-" + descrip2, t + 1);
                    //adding anomaly report to the vector
                    anomalReportVector.push_back(*anomalyReport);
                }
            }
        }

    }
    //returning vector
    return anomalReportVector;
}

//add Info about the correlation in case it a circle
correlatedFeatures HybridAnomalyDetector::addDataOfCircle(Point **arrayOfPoints,int size,correlatedFeatures corrFeatures){
    //create minimal circle according to the correlative vectors
    Circle minCircle=findMinCircle(arrayOfPoints,size);
    //add the radius of the minimal circle to the struct
    corrFeatures.radius= minCircle.radius;
    //same just with center
    corrFeatures.center= &(minCircle.center);
    //updating the indicator to know at the online if a struct is of circle or line
    corrFeatures.isCircle= true;
    corrFeatures.threshold = minCircle.radius*1.1;
    //returning the struct we just now updated
    return corrFeatures;
}





