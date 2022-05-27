#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"

// seg fault liegt an speicher der entweder nicht zugeteilt ist oder keine permission für den Speicher

const char* buildDate = __DATE__;
const char* buildTime = __TIME__;
const int buildVersion = 2;

const char* commands[] = {"--help", "--version", "--prog1", "--prog2", "--prog3", ".bin"};

uint32_t stack[12];
uint32_t* topOfStack;
uint32_t stackSize;

void pushStack(uint32_t data){

    if (stackSize == sizeof(stack) / sizeof(uint32_t)){
        exit(printf("Stack overflow!!"));
    }

    *topOfStack = data;
    stackSize++;
    topOfStack++;
}

uint32_t popStack(){

    if (stackSize == 0){
        exit(printf("Stack empty!"));
    }

    topOfStack--;
    uint32_t data = *topOfStack;
    *topOfStack = 0;
    stackSize--;
    return data;
}

uint32_t readInt(){
    int input;
    scanf("%d", &input);
    //printf("input = %i\n", input);
    return input;
}

void openFile(char path[]){

    char mode = 'r';
    FILE* file = fopen(path, &mode);

    if (file != NULL){
        printf("could load file!\n");
    }
    else printf("could not open file\n"); exit(99);
}

char readChar(){
    char input;
    scanf("%c", &input);
    //printf("input = %c\n", input);
    return input;
}

void printCommands(uint32_t progCode[], uint32_t size){

    for (int i = 0; i <= size; ++i) {

        uint32_t instruction = progCode[i];
        uint32_t command = instruction >> 24;
        char commandStr[10];
        uint32_t immediate = IMMEDIATE(instruction);

        switch (command) {

            default: immediate = 0;
                break;

            case 0: strcpy(commandStr, "halt");
                break;

            case 1: strcpy(commandStr, "pushc");
                break;

            case 2: strcpy(commandStr, "add");
                break;

            case 3: strcpy(commandStr, "sub");
                break;

            case 4: strcpy(commandStr, "mul");
                break;

            case 5: strcpy(commandStr, "div");
                break;

            case 6: strcpy(commandStr, "mod");
                break;

            case 7: strcpy(commandStr, "rdint");
                break;

            case 8: strcpy(commandStr, "wrint");
                break;

            case 9: strcpy(commandStr, "rdchr");
                break;

            case 10: strcpy(commandStr, "wrchr");
                break;

            case 11: strcpy(commandStr, "pushg");
                break;

            case 12: strcpy(commandStr, "popg");
                break;

            case 13: strcpy(commandStr, "asf");
                break;

            case 14: strcpy(commandStr, "rsf");
                break;

            case 15: strcpy(commandStr, "pushl");
                break;

            case 16: strcpy(commandStr, "popl");
                break;

            case 17: strcpy(commandStr, "ep");
                break;

            case 18: strcpy(commandStr, "ne");
                break;

            case 19: strcpy(commandStr, "lt");
                break;

            case 20: strcpy(commandStr, "le");
                break;

            case 21: strcpy(commandStr, "gt");
                break;

            case 22: strcpy(commandStr, "ge");
                break;

            case 23: strcpy(commandStr, "jmp");
                break;

            case 24: strcpy(commandStr, "brf");
                break;

            case 25: strcpy(commandStr, "brt");
                break;

            case 26: strcpy(commandStr, "call");
                break;

            case 27: strcpy(commandStr, "ret");
                break;

            case 28: strcpy(commandStr, "drop");
                break;

            case 29: strcpy(commandStr, "pushr");
                break;

            case 30: strcpy(commandStr, "popr");
                break;

            case 31: strcpy(commandStr, "dup");
                break;
        }

        printf("%03i:\t%s", i, commandStr);

        if (immediate > 0) printf("\t%i", SIGN_EXTEND(immediate));

        printf("\n");
    }

}

void exec(uint32_t commandCode[]){
    bool stop = false;
    uint32_t progCounter = 0;
    uint32_t* commandTable = commandCode;

    while (!stop){

        uint32_t instrucion = commandTable[progCounter];
        uint32_t immediate = IMMEDIATE(instrucion);
        instrucion = instrucion >> 24;

        switch(instrucion) {

            default:
                break;

            case HALT: {
                stop = true;
                break;
            }

            case PUSHC:{
                pushStack(SIGN_EXTEND(immediate));
                break;
            }

            case ADD:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = ADDITION(val1, val2);
                pushStack(result);
                break;
            }

            case SUB:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = SUBTRACTION(val1, val2);
                pushStack(result);
                break;
            }

            case MUL:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = MULTIPILCATION(val1, val2);
                pushStack(result);
                break;
            }

            case DIV:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = DIVISION(val1, val2);
                pushStack(result);
                break;
            }

            case MOD:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = MODULO(val1, val2);
                pushStack(result);
                break;
            }

            case RDINT:{
                uint32_t input = readInt();
                uint32_t hexCode = IMMEDIATE(input);
                pushStack(SIGN_EXTEND(hexCode));
                break;
            }

            case WRINT:{
                uint32_t output = popStack();
                //output = IMMEDIATE(output);
                printf("%i", output);
                break;
            }

            case RDCHR:{
                char input = readChar();
                uint32_t hexCode =IMMEDIATE(input);
                pushStack(hexCode);
                break;
            }

            case WRCHR:{
                uint32_t output = popStack();
                //output = IMMEDIATE(output);
                printf("%c", output);
                break;
            }
        }

        progCounter++;
    }

}

void commandResponse(char* incomeCommand[], int arraySize){

    int cmdType = -2;

    // Wenn min eine Optiopn angegeben wid greift das if
    if(arraySize > 1) {
        cmdType = -1;

        const char dot = '.';
        char* leftOver;

        leftOver = strrchr(incomeCommand[1], dot);
        //printf("leftover = %s\n", leftOver);

        //For vergleicht das erste Argument mit der commands liste
        for (int i = 0; i < sizeof(commands) / sizeof(char *); i++) {

            int cmdCmp;

            if (leftOver == NULL) {
                cmdCmp = strcmp(commands[i], incomeCommand[1]);
            }

            else {
                cmdCmp = strcmp(commands[i], leftOver);
            }

            if (cmdCmp == 0) { cmdType = i; }
        }
    }

    switch (cmdType) {

        default: {
            VMSTART
            VMSTOP
            break;
        }

        case -1:{
            printf("unknown command line argument \x1B[31m'%s'\033[0m, try \x1B[31m'./njvm --help'\033[0m\n", incomeCommand[1]);
            break;
        }

        case 0: {
            printf("usage: ./njvm [options] <code file>"
                   "\n--prog1\tselect program 1 to execute"
                   "\n--prog2\tselect program 2 to execute"
                   "\n--prog3\tselect program 3 to execute"
                   "\n--version\tshow version and exit"
                   "\n--help\t\tshow this help and exit"
                   "\n<code file> opens the file and execute");
            break;
        }

        case 1: {
            printf("Ninja Virtual Machine version %i (compiled %s, %s)\n",buildVersion, buildDate, buildTime);
            break;

        }

        case 2:{
            VMSTART
            //printf("prog1\n");
            printCommands(progCode1, 10);
            exec(progCode1);
            VMSTOP
            break;
        }

        case 3:{
            VMSTART
            //printf("prog2\n");
            printCommands(progCode2, 8);
            exec(progCode2);
            VMSTOP
            break;
        }

        case 4:{
            VMSTART
            //printf("prog3\n");
            printCommands(progCode3, 4);
            exec(progCode3);
            VMSTOP
            break;
        }

        case 5:{
            VMSTART
            printf("file can be opened!\n");
            openFile(incomeCommand[1]);
            VMSTOP
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    topOfStack = stack;
    stackSize = 0;

    commandResponse(argv, argc);

    return 0;
}
