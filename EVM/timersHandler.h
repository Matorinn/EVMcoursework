//
// Created by mato on 28.05.2020.
//

#ifndef EVM_TIMERSHANDLER_H
#define EVM_TIMERSHANDLER_H

extern int instructionCounter;

void runhandler(int signo);
void resetHandler(int signo);

#endif //EVM_TIMERSHANDLER_H
