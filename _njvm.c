#include <stdio.h>
#include <string.h>
#include <stdint.h>

// seg fault liegt an speicher der entweder nicht zugeteilt ist oder keine permission für den Speicher

const char* buildDate = __DATE__;
const char* buildTime = __TIME__;
const int buildVersion = 1;

const char* commands[] = {"--help", "--version"};

struct stack{
    int size;
    int stack[];
};


void commandResponse (char* incomeCommand[], int arraySize){

    int cmdType = -2;

    if(arraySize > 1) {
        cmdType = -1;

        for (int i = 0; i < sizeof(commands) / sizeof(char *); i++) {

            int cmdCmp = strcmp(commands[i], incomeCommand[1]);
            if (cmdCmp == 0) { cmdType = i; }
        }
    }

    switch (cmdType) {

        default: {
            printf("Ninja Virtual Machine started\n");
            printf("Ninja Virtual Machine stopped\n");
            break;
        }

        case -1:{
            printf("unknown command line argument \x1B[31m'%s'\033[0m, try \x1B[31m'./njvm --help'\033[0m\n", incomeCommand[1]);
            break;
        }

        case 0: {
            printf("usage: ./njvm [option] [option] ...\n--version\tshow version and exit\n"
                   "--help\t\tshow this help and exit\n");
            break;
        }

        case 1: {
            printf("Ninja Virtual Machine version %i (compiled %s, %s)\n",buildVersion, buildDate, buildTime);
            break;

        }

    }
}

int main(int argc, char *argv[]) {

    commandResponse(argv, argc);

    return 0;
}
