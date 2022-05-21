#include <stdio.h>
#include <string.h>

const char *commands[] = {"--help", "--version"};

void cmdOutput(int x){
    printf("lol");
}



int main(int argc, char *argv[]) {

    for (int i = 1; i < argc; ++i) {

        for(int j = 0; j < sizeof(commands); j++){

            int cmd = strcmp(argv[i], commands[j]);
            if(cmd == 0){
                cmdOutput(j);
            }
            else{
                printf("unkown command line argument '%s', try './njvm %s'", argv[i], commands[0]);
            }
        }
    }

    printf("Ninja Virtual Machine started\n");
    printf("Ninja Virtual Machine stopped\n");
    return 0;
}
