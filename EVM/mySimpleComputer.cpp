//
// Created by mato on 21.05.2020.
//

#include <iostream>
#include "mySimpleComputer.h"

const short memorySize = 100;
short memory[memorySize];
__uint8_t registerr;
short accumulator;


void sc_memoryInit() {
    for (int i = 0; i < memorySize; i++) {
        memory[i] = 0;
    }
}

int sc_memorySet(short address, short value) {
    if (address >= memorySize || address < 0) {
        sc_regSet(SC_OUT_OF_BOUNDS, 1);
        return -1;
    }
    memory[address] = value;
    return 0;
}

int sc_memoryGet(short address, short &value) {
    if (address >= memorySize || address < 0) {
        sc_regSet(SC_OUT_OF_BOUNDS, 1);
        return -1;
    }
    value = memory[address];
    return 0;
}

int sc_memorySave(char *filename) {
    FILE *fp;
    if ((fp = fopen(filename, "wb")) == NULL) {
        printf("Cannot open file.");
        return -1;
    }
    fwrite(memory, sizeof(memory), 1, fp);
    fclose(fp);
    return 0;
}

int sc_memoryLoad(char *filename) {
    FILE *fp;
    if ((fp = fopen(filename, "rb")) == NULL) {
        printf("Cannot open file.");
        return -1;
    }
    fread(memory, sizeof(memory), 1, fp);
    fclose(fp);
    return 0;
}

void sc_regInit() {
    registerr = 0;
}

int sc_regSet(short position, short value) {
    if (position < 0 || position > 4) {
        sc_regSet(SC_OUT_OF_BOUNDS, 1);
        return -1;
    }
    if (value > 1 || value < 0) {
        sc_regSet(SC_STACK_OVERFLOW, 1);
        return -1;
    }
    if (value == 0) {
        registerr &= ~(1 << position);
    } else {
        registerr |= (value << position);
    }
    return 0;
}

int sc_regGet(short position, short &value) {
    if (position < 0 || position > 4) {
        sc_regSet(SC_OUT_OF_BOUNDS, 1);
        return -1;
    }
    if (position == 0) {
        uint8_t copy = registerr;
        copy <<= 7;
        value = (copy >> 7);
    } else {
        uint8_t copy = registerr;
        copy <<= (7 - position);
        value = (copy >> 7);
    }
    return 0;
}

int sc_commandEncode(short command, short operand, short &value) {
    if (!(command > 0x9 && command < 0x12) && !(command > 0x19 && command < 0x22) &&
        !(command > 0x29 && command < 0x34) && !(command > 0x39 && command < 0x77) && command != 0x64) {
        //TODO: нужны ли флаги?
        sc_regSet(SC_WRONG_COMMAND, 1);
        return -1;
    }
    if (operand < 0 || operand >= memorySize) {
        //TODO: нужны ли флаги?
        sc_regSet(SC_OUT_OF_BOUNDS, 1);
        return -1;
    }
    value = 128;
    value *= command;
    value += operand;
    return 0;
}

int sc_commandDecode(short value, short &command, short &operand) {
    if ((value >> 14 & 1))
        return -1;
    command = value / 128;
    operand = value % 128;
//    if (!(command > 0x9 && command < 0x12) && !(command > 0x19 && command < 0x22) && !(command > 0x29 && command < 0x34) && !(command > 0x39 && command < 0x77)) {
//        sc_regSet(SC_WRONG_COMMAND, 1);
//        return -1;
//    }
    if (operand < 0 || operand >= memorySize) {
        sc_regSet(SC_OUT_OF_BOUNDS, 1);
        return -1;
    }
    return 0;
}