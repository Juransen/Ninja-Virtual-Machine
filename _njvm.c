#include <stdio.h>
#include <string.h>

const char* commands[] = {"--help", "--version"};

void commandResponse (char* incomeCommand[]){

    int cmdType = -1;

    int arraySize = sizeof(incomeCommand) / sizeof(char*);

    if(arraySize > 1) {

        for (int i = 0; i < sizeof(commands) / sizeof(char *); i++) {
            int cmdCmp = strcmp(commands[i], incomeCommand[1]);

            if (cmdCmp == 0) { cmdType = i; }

        }
    }


    switch (cmdType) {
        case -1: {
            printf("-1\n");
        }
        case 0: {
            printf("0\n");
        }
        case 1: {
            printf("1\n");
        }

    }
}

/*
 *  for (int i = 1; i < argc; i++) {

        for(int j = 0; j < sizeof(commands) / sizeof(char*); j++){  //expresion in methode auslagern

            int cmd = strcmp(argv[i], commands[j]);
            if(cmd == 0){
                cmdOutput(j);
            }
            else{
                printf("unkown command line argument '%s', try './njvm %s'", argv[i], commands[0]);
            }
        }
    }
 *
 */


// seg fault liegt an speicher der entweder nicht zugeteilt ist oder keine permission für den Speicher


int main(int argc, char *argv[]) {

    commandResponse(argv);

    printf("Ninja Virtual Machine started\n");
    printf("Ninja Virtual Machine stopped\n");
    return 0;
}
