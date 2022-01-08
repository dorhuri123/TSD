#include "CLI.h"
//constructor
CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
    this->dataClass = new DataClass(vector<AnomalyReport>(),0.9,0.0);
    this->commands[0] = new UploadFile(dio,"1.upload a time series csv file\n",dataClass);
    this->commands[1] = new Correlation(dio,"2.algorithm settings\n",dataClass);
    this->commands[2] = new AnomalyAlgo(dio,"3.detect anomalies\n",dataClass);
    this->commands[3] = new PrintAnomalyReport(dio,"4.display results\n",dataClass);
    this->commands[4] = new AnalysisAnomaly(dio,"5.upload anomalies and analyze results\n",dataClass);
    this->commands[5] = new ExitCommand(dio,"6.exit\n",dataClass);
}

//print the "menu" to the client (the classes)
void CLI::print(Command** arrayOfCommands) {
    this->dio->write("Welcome to the Anomaly Detection Server.\n");
    this->dio->write("Please choose an option:\n");
    for (int i = 0; i < 6; i++) {
        this->dio->write(arrayOfCommands[i]->get());
    }
}

//creat and initalize the classes in array and the user choose from menu
void CLI::start() {
    int choice = 0;
    while (choice != 6) {
        print(this->commands);
        choice = stoi(this->dio->read());
        if (choice > 0 && choice < 7) {
            commands[choice - 1]->execute();
        }
    }
}
    //menu
    /*
    while (flag != 0) {
        switch ((int)choiceOfUser) {
            case 1:
                arrayOfCommands[0]->execute();
                break;
            case 2:
                arrayOfCommands[1]->execute();
                break;
            case 3:
                arrayOfCommands[2]->execute();
                break;
            case 4:
                arrayOfCommands[3]->execute();
                break;
            case 5:
                arrayOfCommands[4]->execute();
                break;
            default:
                flag = 0;
                break;
        }
        if(flag == 0){
            break;
        }
        //print menu again
        print(arrayOfCommands);
        //user enter choice again
        this->dio->read(&choiceOfUser);
    }
     */


CLI::~CLI() {
        for(auto & command : commands) {
            delete command;
        }
        delete dataClass;
}



