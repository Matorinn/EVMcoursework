//
// Created by mato on 28.05.2020.
//
#include <iostream>
#include <unistd.h>
#include <regex>
#include "CU.h"
#include "ALU.h"
#include "timersHandler.h"
#include "mySimpleComputer.h"
#include "myTerm.h"
#include "command.h"

using namespace std;

int CU(){
    regex pos(R"([0-9A-Fa-f]{1,4})");
    regex neg(R"([-]{1}[0-9A-Fa-f]{1,4})");
    regex com(R"([+]{1}[0-9A-Fa-f]{1,4})");
    string enter_value;
    short command = 0;
    short operand = 0;
    sc_commandDecode(memory[instructionCounter], command, operand);
    switch (command) {
        case READ:
            short value;
            cin >> enter_value;

            if(regex_match(enter_value, pos)){
                value = (short) stoi(enter_value, nullptr, 16);
                value |= (1 << 14);
                sc_memorySet(operand, value);;
            }
            else if(regex_match(enter_value, neg)){
                value =  stoi(enter_value, nullptr, 16);
                value = ~(value - 1);//стало просто числом
                value |= (1 << 14); // стало не командой внутри системы СК
                value ^= 0b11111111111111;
                value += 1;
                sc_memorySet(operand, value);
            }
            else if(regex_match(enter_value, com)){
                value = (short) stoi(enter_value, nullptr, 16);
                short temp;
                if(sc_commandEncode(((value >> 8) & 0xFF), (value & 0xFF),temp) == -1)
                    return -1;
                sc_memorySet(operand, value);
            }else{
                return -1;
            }
            instructionCounter++;
            return 0;
        case WRITE:
            short value0;
            sc_memoryGet(operand, value0);
            mt_gotoXY(1, 25);
            if (((value0 >> 14) & 1) == 0) {  // команда
                std::cout << "+";
                printf("%04X", value0);
            }
            else{
                if (((value0 >> 13) & 1) == 0) {
                    std::cout << " ";
                    value0 &= ~(1 << 14);
                    printf("%04X", value0);
                }
                else{
                    std::cout << "-";
                    value0 = (value0 - 1) ^ 0b11111111111111;
                    value0 &= ~(1 << 14);
                    printf("%04X", value0);
                }
            }
            instructionCounter++;
            return 0;
        case LOAD:
            sc_memoryGet(operand, accumulator);
            instructionCounter++;
            return 0;
        case STORE:
            sc_memorySet(operand, accumulator);
            instructionCounter++;
            return 0;
        case JUMP:
            instructionCounter = operand;
            return 0;
        case JNEG:
            if (((accumulator >> 13) & 1) == 1){
                instructionCounter = operand;
            } else
                instructionCounter++;
            return 0;
        case JZ:
            if (accumulator == 0b100000000000000) {
                instructionCounter = operand;
            }else
                instructionCounter++;
            return 0;
        case HALT:
            sc_regSet(SC_IGNORE_CLOCK, 1);
            return 0;
        default:
            ALU(command, operand);
    }
    return 0;
}
