

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"


using namespace std;
//data class- it's a shared class between all the other class in order to share data,
// the class it's in a field of any other class
class DataClass {
public:
    //the results of the algorithem
    vector<AnomalyReport> anomalReportVector;
    //the correlation
    float updatedCorrelation = 0;
    int numberOfLines;
};

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
    DataClass *dataClass;
    string name;
public:
    Command(DefaultIO *dio) : dio(dio) {}

    Command() {}

    virtual string get() = 0;

    virtual void execute() = 0;

    virtual ~Command() {}
};

// implement here your command classes
//class 1- uploading and save the file of the user
class UploadFile : public Command {

public:
    UploadFile(DefaultIO *dio, string name, DataClass *dataClass): Command(dio){
        this->name = name;
        this->dataClass = dataClass;
    }

    string get() {
        return this->name;
    }

//reading the file and make it to a vector of string
    void uploading(vector <string> *someVector) {
        string word = this->dio->read();
        while (word != "done") {
            someVector->push_back(word);
            word = this->dio->read();
        }
        //initializing the number(will be n) indicating number of lines minus the headline
        this->dataClass->numberOfLines = someVector->size() - 1;
        //print message
        this->dio->write("Upload complete.\r\n");
    }

//convert the vector of string to a real file
    void saveFile(vector <string> *someFile, string nameOfFile) {
        //enter name of the file and make file
        std::ofstream outFile(nameOfFile);
        for (const auto &e : *someFile) outFile << e << "\r\n";
    }

//the main function, calling the help function to uplaod and save the file
    void execute() {
        vector <string> trainFile;
        vector <string> testFile;
        this->dio->write("Please upload your local train CSV file.\r\n");
        uploading(&trainFile);
        saveFile(&trainFile, "anomalyTrain.csv");
        this->dio->write("Please upload your local test CSV file.\r\n");
        uploading(&testFile);
        saveFile(&testFile, "anomalyTest.csv");
    }
};

//another class 2-choose correaltion for the algorithm
class Correlation : public Command {
public:
    Correlation(DefaultIO *dio, string name, DataClass *dataClass) : Command(dio) {
        this->name = name;
        this->dataClass = dataClass;
    }

    string get() {
        return this->name;
    }

    //the main function
    void execute() {
        //default correlation
        float updatedCorrelation = 0;
        this->dio->write("The current correlation threshold is 0.9\r\n");
        this->dio->write("Type a new threshold\r\n");

        this->dio->read(&updatedCorrelation);
        while ((updatedCorrelation < 0) || (updatedCorrelation > 1)) {
            this->dio->write("please choose a value between 0 and 1.\r\n");
            this->dio->read(&updatedCorrelation);
        }
        //saving the correlation
        this->dataClass->updatedCorrelation = updatedCorrelation;
    }
};

//another class 3-the hybrid anomaly detection algorithm,here we run and detect abnormaly
class AnomalyAlgo : public Command {
public:
    AnomalyAlgo(DefaultIO *dio, string name, DataClass *dataClass): Command(dio) {
        this->name = name;
        this->dataClass = dataClass;
    }

    string get() {
        return this->name;
    }

//the main function
    void execute() {
        float defaultCorrelation = 0.9;
        HybridAnomalyDetector hybridAnomalyDetection(defaultCorrelation);
        //check if we have updat correalion from the user
        if (this->dataClass->updatedCorrelation != 0) {
            hybridAnomalyDetection.setCorllation(dataClass->updatedCorrelation);
        }
        //convert the file to timeseries and stariong the algo
        hybridAnomalyDetection.learnNormal(TimeSeries("anomalyTrain.csv"));
        //continue the algo and save the vector with the results to the new class
        this->dataClass->anomalReportVector = hybridAnomalyDetection.detect(TimeSeries("anomalyTest.csv"));
        //message printed
        this->dio->write("anomaly detection complete.\r\n");
    }
};

//another class 4-print the results to the user
class PrintAnomalyReport : public Command {
public:
    PrintAnomalyReport(DefaultIO *dio, string name, DataClass *dataClass): Command(dio) {
        this->name = name;
        this->dataClass = dataClass;
    }

    string get() {
        return this->name;
    }

    void execute() {
        //print from dataclass
        for (int i = 0; i < this->dataClass->anomalReportVector.size(); i++) {
            this->dio->write(this->dataClass->anomalReportVector[i].timeStep);
            this->dio->write("  ");
            this->dio->write(this->dataClass->anomalReportVector[i].description);
            this->dio->write("\r\n");
        }
        this->dio->write("Done.\r\n");
    }
};

//another class 5-compar my results vs the user file results and print the differnces/similarites
class AnalysisAnomaly : public Command {
public:
    AnalysisAnomaly(DefaultIO *dio, string name, DataClass *dataClass): Command(dio){
        this->name = name;
        this->dataClass = dataClass;
    }

    string get() {
        return this->name;
    }

//the main function
    void execute() {
        vector <string> fileOfTheClient;
        //the begining of the time report sequence
        int timeStepBefore;
        //the ending of the time report sequence
        int timeStepAfter;
        //each pair has a begin and end of time step of the report of the user file
        vector <pair<int, int>> fileOfTheClientConverted;
        //the vector with my results
        // vector<AnomalyReport> anomalyReport;
        //each pair has a begin and end of time step of the report my file
        vector <pair<int, int>> timeStep;
        //number of sequence (size of the vector 'fileOfTheClientConverted')
        int P;
        //number of true positive
        float TP = 0;
        //number of false positive
        float FP = 0;
        //number of lines of the file of the client
        int N = this->dataClass->numberOfLines;
        //message
        this->dio->write("Please upload your local anomalies file.\r\n");
        string word = this->dio->read();
        while (word != "done") {
            fileOfTheClient.push_back(word);
            word = this->dio->read();
        }
        //print message
        this->dio->write("Upload complete.\r\n");
        //for convenience i copy the vector to ano)ther
        vector<AnomalyReport> anomalyReport = this->dataClass->anomalReportVector;

        //checking sequence of report and initilaize the vector to be pairs of int of time step
        for (int i = 0; i < anomalyReport.size(); i++) {
            timeStepBefore = anomalyReport[i].timeStep;
            timeStepAfter = anomalyReport[i].timeStep;
            //need to fix the i so we wont miss element
            while (i + 1 < anomalyReport.size() && anomalyReport[i].description == anomalyReport[i + 1].description &&
                   anomalyReport[i].timeStep == anomalyReport[i + 1].timeStep - 1) {
                timeStepAfter++;
                i++;
            }
            timeStepAfter++;
            timeStep.push_back(std::make_pair(timeStepBefore, timeStepAfter));
        }

        //convert string to int and add t new vector of pair of int(range of anormaly)
        for (int i = 0; i < fileOfTheClient.size(); i++) {
            fileOfTheClientConverted.push_back(std::make_pair(stoi(fileOfTheClient[i].substr(0, fileOfTheClient[i].find(','))),
                                                              stoi(fileOfTheClient[i].substr(fileOfTheClient[i].find(',') + 1))));
        }
        //check the algorithm with the file of the client
        for (int j = 0; j < timeStep.size(); j++) {
            bool indicator = false;
            for (int i = 0; i < fileOfTheClientConverted.size(); i++) {
                if(timeStep[j].first> fileOfTheClientConverted[i].second &&
                timeStep[j].second > fileOfTheClientConverted[i].second){
                    continue;
                }
                else if(fileOfTheClientConverted[i].first > timeStep[j].second &&
                fileOfTheClientConverted[i].first > timeStep[j].first){
                    continue;
                }
                else{
                   indicator = true;
                }
            }
            if (indicator == false)
                FP++;
            else {
                TP++;
            }
        }
        P = fileOfTheClient.size();
        //claculating N, iterat over the vector of the client and decrease from N all he timestep with report
        for (vector <pair<int,int>>::const_iterator it = fileOfTheClientConverted.begin();
        it != fileOfTheClientConverted.end(); it++){
            //min using from N all the range with report
            N = N - (it->second - it->first);
        }
        //print message with results
        float result1 = TP / P;
        float result2 = FP / N;
        result1 = floorf(result1 * 1000) / 1000;
        result2 = floorf(result2 * 1000) / 1000;
        this->dio->write("True Positive Rate: ");
        this->dio->write(result1);
        this->dio->write( "\r\n");
        this->dio->write("False Positive Rate: ");
        this->dio->write(result2);
        this->dio->write( "\r\n");
    }
};



#endif /* COMMANDS_H_ */
