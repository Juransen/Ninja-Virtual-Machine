//die Header Datei für Makros
#define VMSTART printf("Ninja Virtual Machine started\n");
#define VMSTOP printf("Ninja Virtual Machine stopped\n");


//vm instructions

#define IMMEDIATE(x) ((x) & 0x00FFFFFF)
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i))

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
#define PUSHG 11
#define POPG 12
#define ASF 13
#define RSF 14
#define PUSHL 15
#define POPL 16
#define EQ 17
#define NE 18
#define LT 19
#define LE 20
#define GT 21
#define GE 22
#define JMP 23
#define BRF 24
#define BRT 25
#define CALL 26
#define RET 27
#define DROP 28
#define PUSHR 29
#define POPR 30
#define DUP 31

#define ADDITION(val1, val2) val1 + val2
#define SUBTRACTION(val1, val2) val1 - val2
#define MULTIPILCATION(val1, val2) val1 * val2
#define DIVISION(val1, val2) val1 / val2
#define MODULO(val1, val2) val1 % val2

#define EQUALS(val1, val2) val1 == val2
#define NOTEQUALS(val1, val2) val1 != val2
#define LOWERTHAN(val1, val2) val1 < val2
#define LOWEREQUALS(val1, val2) val1 <= val2
#define GREATERTAHN(val1, val2) val1 > val2
#define GREATEREQUALS(val1, val2) val1 >= val2


uint32_t progCode1[] = {
        (PUSHC << 24) | IMMEDIATE(3),
        (PUSHC << 24) | IMMEDIATE(4),
        (ADD << 24),
        (PUSHC << 24) | IMMEDIATE(10),
        (PUSHC << 24) | IMMEDIATE(6),
        (SUB << 24),
        (MUL << 24),
        (WRINT << 24),
        (PUSHC << 24) | IMMEDIATE(10),
        (WRCHR << 24),
        (HALT << 24)
};

uint32_t progCode2[] ={
        (PUSHC << 24) | IMMEDIATE(-2),
        (RDINT << 24),
        (MUL << 24),
        (PUSHC << 24) | IMMEDIATE(3),
        (ADD << 24),
        (WRINT << 24),
        (PUSHC << 24) | IMMEDIATE('\n'),
        (WRCHR << 24),
        (HALT << 24)
};

uint32_t progCode3[] = {
        (RDCHR << 24),
        (WRINT << 24),
        (PUSHC << 24) | IMMEDIATE('\n'),
        (WRCHR << 24),
        (HALT << 24)
};