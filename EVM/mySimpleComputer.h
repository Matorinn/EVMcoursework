//
// Created by mato on 21.05.2020.
//

#ifndef EVMLABS_MYSIMPLECOMPUTER_H
#define EVMLABS_MYSIMPLECOMPUTER_H

#define SC_STACK_OVERFLOW 0
#define SC_DIVISION_BY_ZERO 1
#define SC_OUT_OF_BOUNDS 2
#define SC_IGNORE_CLOCK 3
#define SC_WRONG_COMMAND 4

extern short memory[];
extern __uint8_t registerr;
extern short accumulator;

void sc_memoryInit();

int sc_memorySet(short address, short value);

int sc_memoryGet(short address, short &value);

int sc_memorySave(char *filename);

int sc_memoryLoad(char *filename);

void sc_regInit();

int sc_regSet(short position, short value);

int sc_regGet(short position, short &value);

int sc_commandEncode(short command, short operand, short &value);

int sc_commandDecode(short value, short &command, short &operand);

#endif //EVMLABS_MYSIMPLECOMPUTER_H
