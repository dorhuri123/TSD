

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"


using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here
};

// you may add here helper classes


// you may edit this class
class Command {
protected:
    DefaultIO *dio;
protected:
    DataClass *dataClass;
protected:
    String name;
public:
    Command(DefaultIO *dio) : dio(dio) {}

    virtual string get() = 0;

    virtual void execute() = 0;

    virtual ~Command() {}
};

// implement here your command classes
//class 1- uploading and save the file of the user
class UploadFile : public Command {

public:
    UploadFile(DefaultIO *dio, String name, DataClass *dataClass) {
        Command(dio);
        this.name = name;
        this->dataClass = dataClass;
    }

    string get() {
        return this.name;
    }
//reading the file and make it to a vector of string
    void uploading(vector <string> *someVector) {
        string word = this->dio->read();
        while (word != "done") {
            *someVector.push_back(word);
            word = this->dio->read();
        }
        this->dataClass.number=*someVector.size()-1;
        //print message
        this->dio->write("Upload complete.");
    }
//convert the vector of string to a real file
    void saveFile(vector <string> *someFile, string nameOfFile) {
        //enter name of the file and make file
        std::ofstream outFile(nameOfFile);
        for (const auto &e : someFile) outFile << e << "\n";
    }
//the main function, calling the help function to uplaod and save the file
    void execute() {
        vector <string> trainFile;
        vector <string> testFile;
        this->dio->write("Please upload your local train CSV file.");
        uploading(&trainFile);
        saveFile(&trainFile, "anomalyTest.csv");
        this->dio->write("Please upload your local test CSV file.");
        uploading(&testFile);
        saveFile(&testFile, "anomalyTrain.csv");
    }
};

//another class 2-choose correaltion for the algorithm
class Correlation : public Command {
public:
    Correlation(DefaultIO *dio, String name, DataClass *dataClass) {
        Command(dio);
        this.name = name;
        this->dataClass = dataClass;
    }

    string get() {
        return this.name;
    }
//the main function
    void execute() {
        //default correalion
        float updatedCorrelation = 0;
        this->dio->write("The current correlation threshold is 0.9\n");
        this->dio->read(updatedCorrelation);
        while ((updatedCorrelation < 0) || (updatedCorrelation > 1)) {
            this->dio->write("please choose a value between 0 and 1.\n");
            this->dio->read(updatedCorrelation);
        }
        //saving the correlation
        this->dataClass.updateCorrelation = updatedCorrelation;

    }
};

//another class 3-the hybrid anomaly detection algorithm,here we run and detect abnormaly
class AnomalyAlgo : public Command {
public:
    AnomalyAlgo(DefaultIO *dio, String name, DataClass *dataClass) {
        Command(dio);
        this.name = name;
        this->dataClass = dataClass;
    }

    string get() {
        return this.name;
    }
//the main function
    void execute() {
        float defaultCorrelation = 0.9;
        //check if we have updat correalion from the user
        if(this->dataClass.updatecorrelation!=0){
            HybridAnomalyDetection hybridAnomalyDetection(dataClass.updatecorrelation);
        }
        else{
            HybridAnomalyDetection hybridAnomalyDetection(defaultCorrelation);
        }
        //convert the file to timeseries and stariong the algo
        hybridAnomalyDetection.learnNormal(TimeSeries("anomalyTrain"));
        //continue the algo and save the vector with the results to the new class
        this->dataClass.anomalReport = hybridAnomalyDetection.detect(TimeSeries("anomalyTest"));
        //message printed
        this->dio->write("complete detection anomaly.\n");
    }
};

//another class 4-print the results to the user
class PrintAnomalyReport : public Command {
public:
    PrintAnomalyReport(DefaultIO *dio, String name, DataClass *dataClass) {
        Command(dio);
        this.name = name;
        this->dataClass = dataClass;
    }

    string get() {
        return this.name;
    }

    void execute() {
        //print from dataclass
        for (int i = 0; i < this->dataClass.anomalReport.size(); i++) {
            this->dio->write(this->dataClass.anomalReport[i].timeStep);
            this->dio->write("  ");
            this->dio->write(this->dataClass.anomalReport[i].description);
            this->dio->write("\n");
        }
        this->dio->write("Done.");
    }
};

//another class 5-compar my results vs the user file results and print the differnces/similarites
class AnalysisAnomaly : public Command {
public:
    AnalysisAnomaly(DefaultIO *dio, String name, DataClass *dataClass) {
        Command(dio);
        this.name = name;
        this->dataClass;
    }

    string get() {
        return this.name;
    }

//the main function
    void execute() {
        vector <string> fileOfTheClient;
        //the begining of the time report sequence
        int timeStepBefore;
        //the ending of the time report sequence
        int timeStepAfter;
        //each pair has a begin and end of time step of the report of the user file
        vector <pair<int,int>> fileOfTheClientConverted;
        //the vector with my results
        vector <AnomalyReport> vector;
        //each pair has a begin and end of time step of the report my file
        vector< pair<int, int>> timeStep;
        //number of sequence (size of the vector 'fileOfTheClientConverted')
        int P;
        //number of true positive
        int TP;
        //number of false positive
        int FP;
        //number of lines of the file of the client
        int n=this->dataClass.number;
        int N=this->dataClass.number;
        //message
        this->dio->write("Please upload your local anomalies file.\n");
        //creat instance of class uploadfile
        UploadFile anomalyReportByTheClient;
        //uploading the file of the user
        anomalyReportByTheClient.uploading(&fileOfTheClient);
        //for convenience i copy the vector to another
        vector = this->dataClass.anomalReport;

//checking sequence of report and initilaize the vector to be pairs of int of time step
        for (int i = 0; i < vector.size(); i++) {
            int k = 0;
            timeStepBefore = vector[i].timeStep;
            timeStepAfter = vector[i].timeStep;
            //need to fix the i so we wont miss element
            while (i < vector.size() && vector[i].description == vector[i+1].description &&
                   vector[i].timeStep == vector[i+1].timeStep - 1) {
                timeStepAfter++;
                i++;
            }
                timeStepAfter++;
                timeStep.push_back(std::make_pair(timeStepBefore,timeStepAfter));

        }

//convert string to int and add t new vector of pair of int(range of anormaly)
for(int i=0;i<fileOfTheClient.size(),i++) {
    fileOfTheClientConverted.push_back(std::make_pair(stoi(substr(0,fileOfTheClient[i].find(','))),
                                                      stoi(substr(fileOfTheClient[i].find(',')+1))));
//check the algorithm with the file of the client
//choose the biggest vector to iterate
for( auto const_iterator it = timeStep.begin() ; it != timeStep.end; it++){
        bool indicator = false;
        for (int i = 0; i < fileOfTheClientConverted.size(); i++) {

        if (timeStep[i].first >= fileOfTheClientConverted[i].first)
            if (timeStep[i].second <= fileOfTheClientConverted[i].second) {
                indicator = true;
            }
        if (timeStep[i].first <= fileOfTheClientConverted[i].first)
            if (timeStep[i].second >= fileOfTheClientConverted[i].second) {
                indicator = true;
            }
    }
        if (indicator == false)
            FP++;
        else {
            TP++;
        }
    }
P=fileOfTheClient.size();
//claculating N, iterat over the vector of the client and decrease from N all he timestep with report
for( fileOfTheClientConverted< pair<int, int>> timeStep;::const_iterator it = edges.begin() ; itt != edges.end; it++){
    //minusing from N all the range with report
    N=N-(fileOfTheClientConverted[i].second-fileOfTheClientConverted[i].first);
        }
//message
        this->dio->write("Upload complete.\n");
        //print message with results
    double result1= TP/P;
    double result2= FP/N;
    floorf(result1*1000)/1000;
    floorf(result2*1000)/1000;
        this->dio->write("True Positive Rate: "+result1+"\n");
        this->dio->write("False Positive Rate: "+result2);

    }
};

//data class- its a shared class between all the other class in order to shar data,
// the class its in a field of any other class
class DataClass {
public:
    //the results of the algorithem
    vector <AnomalyReport> anomalReportVector;
public:
    //the correlation
    float updatedCorrelation=0;
    int numberOfLines=0;


};


#endif /* COMMANDS_H_ */
