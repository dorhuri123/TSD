

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO* dio;
    Command* commands[6];
    DataClass* dataClass;
    // you can add data members
public:
    CLI(DefaultIO* dio);
    void start();
    void print(Command** arrayOfCommands);
    virtual ~CLI();
};

#endif /* CLI_H_ */

