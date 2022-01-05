#include "CLI.h"
#include "commands.h"
//constructor
CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
}

//print the "menu" to the client (the classes)
void CLI::print(Command** arrayOfCommands) {
    this->dio->write("Welcome to the Anomaly Detection Server.\r\n");
    this->dio->write("Please choose an option:\r\n");
    for (int i = 0; i < 5; i++) {
        this->dio->write(arrayOfCommands[i]->get());
    }
    this->dio->write("6.exit\r\n");
}

//creat and initalize the classes in array and the user choose from menu
void CLI::start() {
    float choiceOfUser = 0;
    Command* arrayOfCommands[5];
    DataClass dataClass;
    UploadFile uploadFile(this->dio, "1.upload a time series csv file\r\n", &dataClass);
    Correlation correlation(this->dio, "2.algorithm settings\r\n",&dataClass);
    AnomalyAlgo anomalyAlgo(this->dio, "3.detect anomalies\r\n",&dataClass);
    PrintAnomalyReport printAnomalyReport(this->dio, "4.display results\r\n",&dataClass);
    AnalysisAnomaly analysisAnomaly(this->dio, "5.upload anomalies and analyze results\r\n",&dataClass);
//    arrayOfCommands.push_back(&uploadFile);
//    arrayOfCommands.push_back(&correlation);
//    arrayOfCommands.push_back(&anomalyAlgo);
//    arrayOfCommands.push_back(&printAnomalyReport);
//    arrayOfCommands.push_back(&analysisAnomaly);
   // print(arrayOfCommands);
    arrayOfCommands[0] = &uploadFile;
    arrayOfCommands[1] = &correlation;
    arrayOfCommands[2] = &anomalyAlgo;
    arrayOfCommands[3] = &printAnomalyReport;
    arrayOfCommands[4] = &analysisAnomaly;
    print(arrayOfCommands);
    //read the choice from the user
    this->dio->read(&choiceOfUser);
    bool flag = 1;
    //menu
    while (flag != 0) {
        switch (int(choiceOfUser)) {
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
        }
        if(flag == 0){
            break;
        }
        //print menu again
        print(arrayOfCommands);
        //user enter choice again
        this->dio->read(&choiceOfUser);
    }
}


CLI::~CLI() {
}



