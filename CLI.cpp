#include "CLI.h"
#include "commands.h"
//constructor
CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
}
//print the "menu" to the client (the classes)
CLI::print(Command** arrayOfCommands) {
this->dio->write("Welcome to the Anomaly Detection Server.\n"
"Please choose an option:");
for (int i = 0; i < 5; i++) {
this->dio->write(arrayOfCommands[i]->get());
}
}
//creat and initalize the classes in array and the user choose from menu
void CLI::start() {
    float choiceOfUser = 0;
    Command *arrayOfCommands[5];
    DataClass dataClass;
    UploadFile uploadFile(this->dio, "1. upload a time series csv file\n", &dataClass);
    Correlation correlation(this->dio, "2. algorithm settings\n",&dataClass);
    AnomalyAlgo anomalyAlgo(this->dio, "3. detect anomalies\n",&dataClass);
    PrintAnomalyReport printAnomalyReport(this->dio, "4. display results\n",&dataClass);
    AnalysisAnomaly analysisAnomaly(this->dio, "5. upload anomalies and analyze results",&dataClass);
    arrayOfCommands[0] = &uploadFile;
    arrayOfCommands[1] = &correlation;
    arrayOfCommands[2] = &anomalyAlgo;
    arrayOfCommands[3] = &printAnomalyReport;
    arrayOfCommands[4] = &analysisAnomaly;
    print();
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
        //print menu again
        print(arrayOfCommands);
        //user enter choice again
        this->dio->read(&choiceOfUser);
    }
}


CLI::~CLI() {
}



