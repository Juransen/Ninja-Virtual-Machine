//die Header Datei für Makros
#define VMSTART printf("Ninja Virtual Machine started\n");
#define VMSTOP printf("Ninja Virtual Machine stopped\n");


//vm instructions

#define IMMEDIATE(x) ((x) & 0x00FFFFFF)

#define HALT 0
#define PUSHC 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define MOD 6
#define RDINT 7
#define WRINT 8
#define RDCHR 9
#define WRCHR 10

#define ADDITION(val1, val2) val1 + val2
