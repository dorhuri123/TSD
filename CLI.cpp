#include "CLI.h"
//constructor
CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    this->dataClass = new DataClass{HybridAnomalyDetector(),0.9,vector<AnomalyReport>()};
    this->commands[0] = new UploadFile(dio);
    this->commands[1] = new Correlation(dio,dataClass);
    this->commands[2] = new AnomalyAlgo(dio,dataClass);
    this->commands[3] = new PrintAnomalyReport(dio,dataClass);
    this->commands[4] = new AnalysisAnomaly(dio,dataClass);
    this->commands[5] = new ExitCommand(dio);
}

//print the "menu" to the client (the classes)
void CLI::print(Command** arrayOfCommands) {
    this->dio->write("Welcome to the Anomaly Detection Server.\n");
    this->dio->write("Please choose an option:\n");
    for (int i = 0; i < 6; i++) {
        this->dio->write(arrayOfCommands[i]->text);
    }
}

//creat and initalize the classes in array and the user choose from menu
void CLI::start(){
    int input = 0;
    while(input != 6) {
        print(commands);
        input = stoi(this->dio->read());
        if(input >= 1 && input <= 6) {
            this->commands[input - 1]->execute();
        }
    }
}

CLI::~CLI() {
    for (auto &command: commands) {
        delete command;
    }
    delete dataClass;
}