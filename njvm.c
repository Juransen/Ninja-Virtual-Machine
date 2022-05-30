#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"

// seg fault liegt an speicher der entweder nicht zugeteilt ist oder keine permission für den Speicher


const char* buildDate = __DATE__;
const char* buildTime = __TIME__;
const int buildVersion = 4;

const char* options[] = {"--help", "--version", "--debug"};

uint32_t stack[1000];
uint32_t* stackPointer;
uint32_t* framePointer;
//uint32_t frameSize;
uint32_t stackSize;

uint32_t returnValueReg[100];
uint32_t returnValueRegIndex;

uint32_t countOfFrames;
uint32_t frameSizes[50];

uint32_t* commList;
uint32_t* staticDataArea;

uint32_t format;
uint32_t version;
uint32_t countOfInstructions;
uint32_t countOfVars;

bool debugMode;


void pushStack(uint32_t data){

    if (stackSize == sizeof(stack) / sizeof(uint32_t)){
        exit(printf("Stack overflow!!\n"));
    }

    *stackPointer = data;
    stackSize++;
    stackPointer++;
}

uint32_t popStack(){

    if (stackSize == 0){
        exit(printf("Stack empty!\n"));
    }

    stackPointer--;
    uint32_t data = *stackPointer;
    *stackPointer = 0;
    stackSize--;
    return data;
}

void deleteFromStack(){
    if (stackSize == 0){
        exit(printf("Stack empty!\n"));
    }

    stackPointer--;
    *stackPointer = 0;
    stackSize--;
}

uint32_t readInt(){
    int input;
    scanf("%d", &input);
    //printf("input = %i\n", input);
    return input;
}

char readChar(){
    char input;
    scanf("%c", &input);
    //printf("input = %c\n", input);
    return input;
}

void allocateStackFrame(uint32_t space){
    printf("asf start\n");
    pushStack((uint32_t) framePointer); // cast auf ein uint. die adresse des Pointers soll gespeichert werden
    //printf("stackpointerval %i\nFP: %i\n", stackPointer, framePointer);
    framePointer = stackPointer;
    stackPointer = stackPointer + space;
    stackSize += space;
    //frameSize = space;
    frameSizes[countOfFrames] = space;
    countOfFrames++;
    printf("asf stop\n");
}

void releaseStackFrame(){

    stackPointer = framePointer;
    *framePointer = popStack();
    countOfFrames--;
    stackSize -= frameSizes[countOfFrames];

}

void pushLocalVar(uint32_t pos){
    //kopiert von der frame auf den Stack
    printf("stacksize: %i\t stackstart: %i\n", stackSize, (uint32_t) stack);
    printf("pos: %i\t fP: %i\n", pos, (uint32_t) framePointer);
    printf("uint32: %i\n", sizeof(uint32_t));
    uint32_t *destination = framePointer + pos;
    printf("dest: %i\n", destination);
    uint32_t input = *destination;
    printf("inp: %i\n", input);

    //printf("pushl: %i\tstack[%i]\tpos: %i\n", input, *destionantion, pos);

    //pushStack(input);
}

void popLocalVariable(uint32_t pos){
    //pusht vom stack in die frame

    uint32_t output = popStack();
    uint32_t* destinantion = framePointer + pos;

    //printf("popl: %i\t\tStackvalue %i\tpos: %i\n", output, *destinantion, pos);

    *destinantion = output;
}

FILE* openFile(char path[]){

    char mode = 'r';
    FILE* file = fopen(path, &mode);

    if (file == NULL){
        printf("Error: cannot open code file '%s'\n", path);
        exit(99);
    }

    if (debugMode){
        printf("DEBUG: file '%s' loaded", path);
    }

    return file;
}

FILE* readFile(FILE* file){

    uint32_t headerDataField[4];
    fread(headerDataField, 4, 4, file);

    format = headerDataField[0];
    version = headerDataField[1];
    countOfInstructions = headerDataField[2];
    countOfVars = headerDataField[3];

    if (debugMode){
        printf(" (code size = %i, data size = %i)\n", countOfInstructions, countOfVars);
    }

    //printf("format: %c\nversion: %i\ncount of instructions: %i\nvariables: %i\n", format, version, countOfInstructions, countOfVars);

    uint32_t formatIdentfyer = 0x46424a4e; //NJBF in HEX

    uint32_t identifyer = 0;

    if (format == formatIdentfyer) identifyer = 1;

    if(identifyer == 0){
        printf("Format not compatible!\n");
        exit(99);
    }

    if (version != buildVersion){
        printf("file version does not match VM version\n");
        exit(99);
    }

    staticDataArea = malloc(countOfVars * sizeof(uint32_t));

    commList = malloc(countOfInstructions * sizeof(uint32_t));
    fread(commList, sizeof(uint32_t), countOfInstructions, file);

    FILE* openedFile = file;
    return openedFile;
}

void printCommands(uint32_t progCode[], uint32_t size){

    for (int i = 0; i < size; ++i) {

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

        if (command == POPG || command == PUSHG||
            command == PUSHL || command == POPL || immediate > 0){
            printf("\t%i", SIGN_EXTEND(immediate));
        }
        printf("\n");
    }
    printf("\t---end of code---\n");
}

void exec(uint32_t commandCode[]){

    bool stop = false;
    uint32_t progCounter = 0;
    uint32_t* commandTable = commandCode;

    while (!stop){
        printf("fisch\n");
        uint32_t instruction = commandTable[progCounter];
        uint32_t immediate = IMMEDIATE(instruction);
        instruction = instruction >> 24;
        printf("kein fisch\timme: %i\tinstr: %i\n", SIGN_EXTEND(immediate), instruction);

        if (debugMode){
            char commandStr[10];
            switch (instruction) {

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

            bool next = false;

            while (!next) {
                printf("%04i:\t%s", progCounter, commandStr);
                if (instruction == POPG || instruction == PUSHG||
                        instruction == PUSHL || instruction == POPL || immediate > 0){
                    printf("\t%i", SIGN_EXTEND(immediate));
                }
                printf("\n");
                printf("DEBUG: inspect, list, breakpoint, step, run, quit?\n");

                char instr;
                scanf("%s", &instr);

                char x; //kopiere den ersten buchstaben der eingabe in x
                memcpy(&x, &instr, 1);

                switch (x) {

                    default:{
                        break;
                    }

                    case 'i':{
                        printf("DEBUG [inspect]: stack, data?\n");
                        char inspect;
                        scanf("%s", &inspect);
                        char buff;
                        memcpy(&buff, &inspect, 1);

                        switch (buff) {
                            default:
                                break;

                            case 's':{
                                printf("stackinspect not implemented\n");
                                break;
                            }
                            case 'd':{
                                printf("datainspect not implemented\n");
                                break;
                            }
                        }

                        break;
                    }

                    case 'l':{
                        printCommands(commList, countOfInstructions);
                        break;
                    }

                    case 's':{
                        next = true;
                        break;
                    }

                    case 'b':{
                        printf("breakpoint not implemented\n");
                        break;
                    }
                    case 'r':{
                        debugMode = false;
                        next = true;
                        break;
                    }
                    case 'q':{
                        VMSTOP
                        exit(0);
                    }

                }
            }

        }
        printf("superfisch\n");
        progCounter++;
        printf("kugelfisch\n");

        switch(instruction) {

            case HALT:{
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
                printf("start write\n");
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

            case PUSHG:{
                uint32_t input = staticDataArea[immediate];
                pushStack(SIGN_EXTEND(input));
                break;
            }

            case POPG:{
                uint32_t output = popStack();
                staticDataArea[immediate] = output;
                break;
            }

            case ASF:{
                allocateStackFrame(immediate);
                break;
            }

            case RSF:{
                releaseStackFrame();
                break;
            }

            case PUSHL:{
                printf("test\n");
                printf("imme: %i\n", SIGN_EXTEND(immediate));
                pushLocalVar(SIGN_EXTEND(immediate));
                break;
            }

            case POPL:{
                popLocalVariable(immediate);
                break;
            }

            case EQ:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = EQUALS(val1, val2);
                pushStack(result);
                break;
            }

            case NE:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = NOTEQUALS(val1, val2);
                pushStack(result);
                break;
            }

            case LT:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = LOWERTHAN(val1, val2);
                pushStack(result);
                break;
            }

            case LE:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = LOWEREQUALS(val1, val2);
                pushStack(result);
                break;
            }

            case GT:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = GREATERTAHN(val1, val2);
                pushStack(result);
                break;
            }

            case GE:{
                uint32_t val2 = popStack();
                uint32_t val1 = popStack();
                uint32_t result = GREATEREQUALS(val1, val2);
                pushStack(result);
                break;
            }

            case JMP:{
                progCounter = immediate;
                break;
            }

            case BRF:{
                uint32_t output = popStack();
                if (output == 0) progCounter = immediate;
                break;
            }

            case BRT:{
                uint32_t output = popStack();
                if (output == 1) progCounter = immediate;
                break;
            }

            case CALL:{
                pushStack(progCounter);
                progCounter = immediate;
                break;
            }

            case RET:{
                uint32_t retAdress = popStack();
                progCounter = retAdress;
                break;
            }

            case DROP:{
                for (int i = 0; i < immediate; ++i) {
                    deleteFromStack();
                }
                break;
            }

            case PUSHR:{
                returnValueRegIndex--;
                uint32_t retVal = returnValueReg[returnValueRegIndex];
                pushStack(retVal);
                break;
            }

            case POPR:{
                uint32_t retVal = popStack();
                returnValueReg[returnValueRegIndex] = retVal;
                returnValueRegIndex++;
                break;
            }

            case DUP:{
                uint32_t duplicate = popStack();
                pushStack(duplicate);
                pushStack(duplicate);
                break;
            }

            default:
                break;
        }

        printf("FISCHE\n");
    }

    free(commList);
    free(staticDataArea);
}

void commandResponse(char* incomeCommand[], int arraySize){

    //const char* options[] = {"--help", "--version", "--debug"};

    int cmdType = -2;
    int option = -1;
    int file = -1;

    if (arraySize > 1) {
        char optPrefix[2];
        for (int i = 1; i < arraySize; i++) {
            memcpy(optPrefix, incomeCommand[i], 2);

            int cmp = strcmp(optPrefix, "--");
            if (cmp == 0) {
                option = i;
                cmdType = -1;
            }

            if (cmp != 0) {
                file = i;
                cmdType = 3;
            }
        }

        for (int i = 0; i < 3; ++i) {
            if (option >= 0) {
                int cmp = strcmp(options[i], incomeCommand[option]);
                if (cmp == 0) cmdType = i;
            }
        }
    }

    //printf("cmdtype: %i\nopt: %i\nfile: %i\n", cmdType, option, file);


/*    // Wenn min eine Optiopn angegeben wid greift das if
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

        //abgleich des zweiten Arguments, ob debugMode aktiviert werden soll

        if (arraySize > 2){
            if (strcmp(incomeCommand[2], commands[4]) == 0){
                debugMode = true;
                cmdType = 5;
            }
            else{
                printf("Error: unknown option '%s', try ./njvm --help\n", incomeCommand[2]);
                exit(99);
            }
        }
    }*/

    switch (cmdType) {

        default: {
            printf("Error: no code file specified\n");
            break;
        }

        case -1:{
            printf("Error: unknown option '%s', try '%s --help'\n", incomeCommand[option], incomeCommand[0]);
            break;
        }

        case 0: {
            printf("Usage: ./njvm [options] <code file>\nOptions:"
                   "\n--debug\t\tstart virtuel machine in debug mode"
                   "\n--version\tshow version and exit"
                   "\n--help\t\tshow this help and exit\n");
            break;
        }

        case 1: {
            printf("Ninja Virtual Machine version %i (compiled %s, %s)\n",buildVersion, buildDate, buildTime);
            break;

        }

        case 2:{
            debugMode = true;
        }

        case 3:{
            fclose(readFile(openFile(incomeCommand[file])));
            VMSTART
            exec(commList);
            VMSTOP
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    stackPointer = stack;
    framePointer = stack;
    stackSize = 0;
    returnValueRegIndex = 0;
    countOfFrames = 0;
    debugMode = false;

    commandResponse(argv, argc);

    return 0;
}
