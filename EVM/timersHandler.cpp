//
// Created by mato on 28.05.2020.
//
#include <sys/time.h>
#include <unistd.h>
#include <csignal>
#include "mySimpleComputer.h"
#include "CU.h"

int instructionCounter;

void runhandler(int signo){
    short value;
    sc_regGet(SC_IGNORE_CLOCK, value);
    if (value == 0) {
        CU();
        alarm(1);
    }
    //TODO: ПОДУМАТЬ
}

void resetHandler(int signo){
    alarm(0);
    sc_memoryInit();
    sc_regInit();
    sc_regSet(SC_IGNORE_CLOCK, 1);
    instructionCounter = 0;
    accumulator = 0;

}



#include "timersHandler.h"
