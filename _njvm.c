#include <stdio.h>
#include <string.h>

const char* commands[] = {"--help", "--version"};

void commandResponse (char* incomeCommand[], int arraySize){

    int cmdType = -2;

    //printf("%d\n", arraySize);

    if(arraySize > 1) {
        cmdType = -1;

        for (int i = 0; i < sizeof(commands) / sizeof(char *); i++) {
            int cmdCmp = strcmp(commands[i], incomeCommand[1]);

            if (cmdCmp == 0) { cmdType = i; }
        }
    }


    switch (cmdType) {

        default: {
            //printf("default\n");
            printf("Ninja Virtual Machine started\n");
            printf("Ninja Virtual Machine stopped\n");
            break;
        }

        case -1:{
            //printf("-1  invalid command\n");
            printf("unknown command line argument '%s', try './njvm --help'\n", incomeCommand[1]);
            break;
        }

        case 0: {
            //printf("0 help\n");
            printf("usage: ./njvm [option] [option] ...\n--version       show version and exit\n"
                   "--help          show this help and exit\n");
            break;
        }

        case 1: {
            printf("1 version\n");
            printf("\n");
            break;
        }

    }
}



// seg fault liegt an speicher der entweder nicht zugeteilt ist oder keine permission für den Speicher


int main(int argc, char *argv[]) {

    commandResponse(argv, argc);

    return 0;
}
