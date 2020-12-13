//
// Created by mato on 30.05.2020.
//
#include <iostream>
#include <unistd.h>
#include "CU.h"
#include "timersHandler.h"
#include "mySimpleComputer.h"
#include "myTerm.h"
#include "command.h"
#include "ALU.h"

int ALU(short command, short operand){
    switch (command) {
        case ADD:
            short value;
            sc_memoryGet(operand, value);
            if ((((accumulator >> 13) & 1) == 0) && (((value >> 13) & 1) == 0)) { // +аккум и +число в ячейке
                short acc;
                acc = accumulator & ~(1 << 14);
                value &= ~(1 << 14);
                acc += value;
                acc |= (1 << 14);
                accumulator = acc;
            }
            else if ((((accumulator >> 13) & 1) == 1) && (((value >> 13) & 1) == 1)) { // -аккум и -число в ячейке
                short acc;
                acc = (accumulator - 1) ^ 0b11111111111111;
                acc &= ~(1 << 14);
                value = (value - 1) ^ 0b11111111111111;
                value &= ~(1 << 14);
                acc += value;
                acc |= (1 << 14); // стало не командой внутри системы СК
                acc ^= 0b11111111111111;
                acc += 1;
                accumulator = acc;
            }
            else if ((((accumulator >> 13) & 1) == 0) && (((value >> 13) & 1) == 1)){ // +аккум и -число
                short acc;
                acc = accumulator & ~(1 << 14);
                value = (value - 1) ^ 0b11111111111111;
                value &= ~(1 << 14);
                if (acc - value >= 0){
                    acc = abs(acc - value);
                    acc |= (1 << 14);
                    accumulator = acc;
                }else{
                    acc = abs(acc - value);
                    acc |= (1 << 14); // стало не командой внутри системы СК
                    acc ^= 0b11111111111111;
                    acc += 1;
                    accumulator = acc;
                }

            }
            else if ((((accumulator >> 13) & 1) == 1) && (((value >> 13) & 1) == 0)){ // -аккум и +число
                short acc;
                acc = (accumulator - 1) ^ 0b11111111111111;
                acc &= ~(1 << 14);
                value &= ~(1 << 14);
                if (value - acc >= 0){
                    acc = abs(acc - value);
                    acc |= (1 << 14);
                    accumulator = acc;
                }else{
                    acc = abs(acc - value);
                    acc |= (1 << 14); // стало не командой внутри системы СК
                    acc ^= 0b11111111111111;
                    acc += 1;
                    accumulator = acc;
                }

            }
            instructionCounter++;
            return 0;
        case SUB:
            short vals;
            sc_memoryGet(operand, vals);
            if ((((accumulator >> 13) & 1) == 0) && (((vals >> 13) & 1) == 0)) { // +аккум и +число в ячейке
                short acc;
                acc = accumulator & ~(1 << 14);
                vals &= ~(1 << 14);
                if (acc - vals >= 0){
                    acc = abs(acc - vals);
                    acc |= (1 << 14);
                    accumulator = acc;
                }else{
                    acc = abs(acc - vals);
                    acc |= (1 << 14); // стало не командой внутри системы СК
                    acc ^= 0b11111111111111;
                    acc += 1;
                    accumulator = acc;
                }
            }
            else if ((((accumulator >> 13) & 1) == 1) && (((vals >> 13) & 1) == 1)) { // -аккум и -число в ячейке
                short acc;
                acc = (accumulator - 1) ^ 0b11111111111111;
                acc &= ~(1 << 14);
                vals = (vals - 1) ^ 0b11111111111111;
                vals &= ~(1 << 14);
                if (vals - acc >= 0){
                    acc = abs(acc - vals);
                    acc |= (1 << 14);
                    accumulator = acc;
                }else{
                    acc = abs(acc - vals);
                    acc |= (1 << 14); // стало не командой внутри системы СК
                    acc ^= 0b11111111111111;
                    acc += 1;
                    accumulator = acc;
                }
            }
            else if ((((accumulator >> 13) & 1) == 0) && (((vals >> 13) & 1) == 1)){ // +аккум и -число
                short acc;
                acc = accumulator & ~(1 << 14);
                vals = (vals - 1) ^ 0b11111111111111;
                vals &= ~(1 << 14);
                acc += vals;
                acc |= (1 << 14);
                accumulator = acc;
            }
            else if ((((accumulator >> 13) & 1) == 1) && (((vals >> 13) & 1) == 0)){ // -аккум и +число
                short acc;
                acc = (accumulator - 1) ^ 0b11111111111111;
                acc &= ~(1 << 14);
                vals &= ~(1 << 14);
                acc += vals;
                acc |= (1 << 14); // стало не командой внутри системы СК
                acc ^= 0b11111111111111;
                acc += 1;
                accumulator = acc;
            }
            instructionCounter++;
            return 0;
        case DIVIDE:
            short vald;
            sc_memoryGet(operand, vald);
            if (vald == 0x4000){
                sc_regSet(SC_DIVISION_BY_ZERO,1);
                return -1;
            }
            if ((((accumulator >> 13) & 1) == 0) && (((vald >> 13) & 1) == 0)) { // +аккум и +число в ячейке
                short acc;
                acc = accumulator & ~(1 << 14);
                vald &= ~(1 << 14);
                acc /= vald;
                acc |= (1 << 14);
                accumulator = acc;
            }
            else if ((((accumulator >> 13) & 1) == 1) && (((vald >> 13) & 1) == 1)) { // -аккум и -число в ячейке
                short acc;
                acc = (accumulator - 1) ^ 0b11111111111111;
                acc &= ~(1 << 14);
                vald = (vald - 1) ^ 0b11111111111111;
                vald &= ~(1 << 14);
                acc /= vald;
                acc |= (1 << 14); // стало не командой внутри системы СК
                accumulator = acc;
            }
            else if ((((accumulator >> 13) & 1) == 0) && (((vald >> 13) & 1) == 1)){ // +аккум и -число
                short acc;
                acc = accumulator & ~(1 << 14);
                vald = (vald - 1) ^ 0b11111111111111;
                vald &= ~(1 << 14);
                acc /= vald;
                acc |= (1 << 14); // стало не командой внутри системы СК
                acc ^= 0b11111111111111;
                acc += 1;
                accumulator = acc;
            }
            else if ((((accumulator >> 13) & 1) == 1) && (((vald >> 13) & 1) == 0)){ // -аккум и +число
                short acc;
                acc = (accumulator - 1) ^ 0b11111111111111;
                acc &= ~(1 << 14);
                vald &= ~(1 << 14);
                acc /= vald;
                acc |= (1 << 14); // стало не командой внутри системы СК
                acc ^= 0b11111111111111;
                acc += 1;
                accumulator = acc;
            }
            instructionCounter++;
            return 0;
        case MUL:
            short val;
            sc_memoryGet(operand, val);
            if ((((accumulator >> 13) & 1) == 0) && (((val >> 13) & 1) == 0)) { // +аккум и +число в ячейке
                short acc;
                acc = accumulator & ~(1 << 14);
                val &= ~(1 << 14);
                acc *= val;
                acc |= (1 << 14);
                accumulator = acc;
            }
            else if ((((accumulator >> 13) & 1) == 1) && (((val >> 13) & 1) == 1)) { // -аккум и -число в ячейке
                short acc;
                acc = (accumulator - 1) ^ 0b11111111111111;
                acc &= ~(1 << 14);
                val = (val - 1) ^ 0b11111111111111;
                val &= ~(1 << 14);
                acc *= val;
                acc |= (1 << 14); // стало не командой внутри системы СК
                accumulator = acc;
            }
            else if ((((accumulator >> 13) & 1) == 0) && (((val >> 13) & 1) == 1)){ // +аккум и -число
                short acc;
                acc = accumulator & ~(1 << 14);
                val = (val - 1) ^ 0b11111111111111;
                val &= ~(1 << 14);
                acc *= val;
                acc |= (1 << 14); // стало не командой внутри системы СК
                acc ^= 0b11111111111111;
                acc += 1;
                accumulator = acc;
            }
            else if ((((accumulator >> 13) & 1) == 1) && (((val >> 13) & 1) == 0)){ // -аккум и +число
                short acc;
                acc = (accumulator - 1) ^ 0b11111111111111;
                acc &= ~(1 << 14);
                val &= ~(1 << 14);
                    acc *= val;
                    acc |= (1 << 14); // стало не командой внутри системы СК
                    acc ^= 0b11111111111111;
                    acc += 1;
                    accumulator = acc;
            }
            instructionCounter++;
            return 0;
        case NEG:
            short acc;
            acc ^= 0b11111111111111;
            acc += 1;
            accumulator = acc;
            instructionCounter++;
            return 0;
        default:
            sc_regSet(SC_WRONG_COMMAND, 1);
            return -1;
    }
}