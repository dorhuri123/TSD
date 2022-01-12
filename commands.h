#ifndef COMMANDS_H_
#define COMMANDS_H_
#include <string.h>
#include <fstream>
#include <vector>
#include <memory>
#include <sstream>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){};
};

struct DataClass{
public:
    HybridAnomalyDetector hd;
    float desiredThreshHold;
    vector<AnomalyReport> ar;
};



// you may edit this class
class Command {
protected:
    DefaultIO* dio;
public:
    string text;
    Command(DefaultIO* dio):dio(dio){}
    virtual void execute()=0;
    virtual ~Command(){}
};


class UploadFile : public Command {
public:
    UploadFile(DefaultIO* dio):Command(dio) {
        this->text = "1.upload a time series csv file\n";
    }
    void execute() override {
        ofstream trainAnomaly, testAnomaly;
        testAnomaly.open("anomalyTest.csv");
        trainAnomaly.open("anomalyTrain.csv");
        dio->write("Please upload your local train CSV file.\n");
        string input = dio->read();
        // while the read line is not "done"
        while(input != "done") {
            trainAnomaly << input << endl;
            input = dio->read();
        }
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        input = dio->read();
        // while the read line is not "done"
        while(input != "done") {
            testAnomaly << input << endl;
            input = dio->read();
        }
        dio->write("Upload complete.\n");
        trainAnomaly.close();
        testAnomaly.close();
    }
};

class Correlation : public Command {
    DataClass* dataClass;
public:
    Correlation(DefaultIO* dio, DataClass* dataClass): Command(dio) {
        this->text = "2.algorithm settings\n";
        this->dataClass = dataClass;
    }
    void execute() override {
        dio->write("The current correlation threshold is " + to_string(dataClass->desiredThreshHold) + "\n");
        dio->write("Type a new threshold\n");
        float newThreshold;
        newThreshold = stof(dio->read());
        while(newThreshold <= 0 || newThreshold > 1) {
            dio->write("please choose a value between 0 and 1.\n");
            newThreshold = stof(dio->read());
        }
        dataClass->desiredThreshHold = newThreshold;
        dataClass->hd.setCorllation(newThreshold);
    }
};
class AnomalyAlgo : public Command{
    DataClass *dataClass;
    const char* trainCSV = "anomalyTrain.csv";
    const char* testCSV = "anomalyTest.csv";
    const char* detectionCompleteMSG = "anomaly detection complete.\n";
public:
    AnomalyAlgo(DefaultIO* dio, DataClass *dataClass): Command(dio), dataClass(dataClass){
        this->text = "3.detect anomalies\n";
    }
    void execute() override{
        TimeSeries testTimeSeries(testCSV);
        TimeSeries trainTimeSeries(trainCSV);

        dataClass->hd.learnNormal(trainTimeSeries);
        dataClass->ar = dataClass->hd.detect(testTimeSeries);
        dio->write(detectionCompleteMSG);
    }
};

class PrintAnomalyReport : public Command{
    DataClass *sc;
public:
    PrintAnomalyReport(DefaultIO* dio, DataClass *sc): Command(dio), sc(sc){
        this->text = "4.display results\n";
    }
    void execute() override{
        string textStr, done = "done\n";
        for (const auto& report : sc->ar){
            textStr.append((to_string(report.timeStep) + "\t" + report.description + "\n"));
        }
        textStr.append(done);
        dio->write(textStr);
    }
};

class AnalysisAnomaly : public Command{
    string truePositiveComment = "True Positive Rate: ";
    string falsePositiveComment = "False Positive Rate: ";
    string uploadComment = "Please upload your local anomalies file.\n";
    string uploadComplete = "Upload complete.\n";
    DataClass *dataClass;
public:
    AnalysisAnomaly(DefaultIO* dio, DataClass *dataClass): Command(dio), dataClass(dataClass){
        this->text = "5.upload anomalies and analyze results\n";
    }

    void execute() override{
        int P = 0, s, t, FP = 0, TP = 0, startTime, endTime;
        bool ok;
        int  N = TimeSeries("anomalyTest.csv").getData().begin()->second.size();
        float TPrate, FPrate;
        stringstream TPStream, FPStream;
        auto reportVec = report_Vector();
        auto anomalyVec = anomaly_Vector(P, N);
        dio->write(uploadComplete);

        for(const auto& exception_report : reportVec) {
            ok = false;
            startTime = exception_report.first;
            endTime = exception_report.second;
            for(const auto& report : anomalyVec){
                s = report.first;
                t = report.second;
                if ((startTime <= s && endTime >= s) || (startTime >= s && startTime <= t)){
                    TP++;
                    ok = true;
                }
            }
            if (!ok)
                FP++;
        }

        TPrate = (float) floor(TP * 1000 / P) / 1000;
        FPrate = (float) floor(FP * 1000 / N) / 1000;
        TPStream << TPrate;
        FPStream << FPrate;

        dio->write(truePositiveComment + TPStream.str() + "\n");
        dio->write(falsePositiveComment + FPStream.str() + "\n");
    }

    vector<pair<int,int>> report_Vector(){
        int start, cT;
        string currentText;
        vector<pair<int,int>> reportVecor1;
        start = dataClass->ar.begin()->timeStep;
        cT = start;
        currentText = dataClass->ar.begin()->description;
        for(int i = 1; i < dataClass->ar.size() - 1; i++){
            auto report = dataClass->ar[i];
            if (report.description != currentText || report.timeStep != cT + 1 ){
                currentText = report.description;
                pair<int,int> tempPair(start,cT);
                reportVecor1.push_back(tempPair);
                start = report.timeStep;
                cT = start;
            }
            else
                cT = report.timeStep;
        }

        pair<int,int> tempPair(start,cT);
        reportVecor1.push_back(tempPair);

        return reportVecor1;
    }


    vector<pair<int, int>> anomaly_Vector(int &P, int &N) {
        int split, start, end;
        vector<pair<int,int>> anomalyVector1;
        dio->write(uploadComment);
        string s = dio->read();
        // while the read line is not "done"
        while(s != "done") {
            split = s.find(',');
            start = stoi(s.substr(0,split));
            end = stoi(s.substr(split + 1));
            pair<int,int> tempPair(start,end);
            anomalyVector1.push_back(tempPair);
            N = N - (end - start);
            P++;
            s = dio->read();
        }
        return anomalyVector1;
    }
};

class ExitCommand : public Command{
public:
    ExitCommand(DefaultIO* dio) : Command(dio) {
        this->text = "6.exit\n";
    }
    void execute() override {}
};


#endif /* COMMANDS_H_ */