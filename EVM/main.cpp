#include <iostream>
#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myBigChars.h"
#include "keys.h"
#include "myReadKey.h"
#include <csignal>
#include <sys/time.h>
#include <unistd.h>
#include <bitset>
#include "timersHandler.h"
#include "CU.h"
#include <string.h>
#include <regex>
#include <iomanip>

using namespace std;

void drawmenu() {
    mt_clrscr();
    bc_box(1, 1, 61, 12);
    bc_box(62, 1, 22, 3);
    bc_box(62, 4, 22, 3);
    bc_box(62, 7, 22, 3);
    bc_box(62, 10, 22, 3);
    bc_box(1, 13, 46, 10);
    bc_box(48, 13, 36, 10);
    mt_gotoXY(30, 1);
    cout << "Memory";
    mt_gotoXY(66, 1);
    cout << "Accumulator";
    mt_gotoXY(64, 4);
    cout << "InstructionCounter";
    mt_gotoXY(68, 7);
    cout << "Operation";
    mt_gotoXY(68, 10);
    cout << "Flags";
    mt_gotoXY(49, 13);
    cout << "Keys:";
    mt_gotoXY(50, 14);
    cout << "l  - load";
    mt_gotoXY(50, 15);
    cout << "s  - save";
    mt_gotoXY(50, 16);
    cout << "r  - run";
    mt_gotoXY(50, 17);
    cout << "t  - step";
    mt_gotoXY(50, 18);
    cout << "i  - reset";
    mt_gotoXY(50, 19);
    cout << "F5 - accumulator";
    mt_gotoXY(50, 20);
    cout << "F6 - instructionCounter";
    mt_gotoXY(0, 24);
    cout << "Type here:";
}

void drawflags() {
    short value;
    sc_regGet(SC_STACK_OVERFLOW, value);
    if (value == 1) {
        mt_gotoXY(65, 11);
        cout << "S";
    }
    sc_regGet(SC_DIVISION_BY_ZERO, value);
    if (value == 1) {
        mt_gotoXY(67, 11);
        cout << "0";
    }
    sc_regGet(SC_OUT_OF_BOUNDS, value);
    if (value == 1) {
        mt_gotoXY(69, 11);
        cout << "M";
    }
    sc_regGet(SC_IGNORE_CLOCK, value);
    if (value == 1) {
        mt_gotoXY(71, 11);
        cout << "C";
    }
    sc_regGet(SC_WRONG_COMMAND, value);
    if (value == 1) {
        mt_gotoXY(73, 11);
        cout << "E";
    }
}

void drawcell(int i) {
    short value;
    sc_memoryGet(i, value);

    if (((value >> 14) & 1) == 0) {  // команда
        cout << "+";
        printf("%04X", value);
    }
    else{
        if (((value >> 13) & 1) == 0) {
            cout << " ";
            value &= ~(1 << 14);
            printf("%04X", value);
        }
        else{
           cout << "-";
           value = (value - 1) ^ 0b11111111111111;
           value &= ~(1 << 14);
           printf("%04X", value);
        }
    }
}

void drawaccum(){
    short value = accumulator;
    if (((value >> 14) & 1) == 0) {  // команда
        cout << "+";
        printf("%04X", value);
    }
    else{
        if (((value >> 13) & 1) == 0) {
            cout << " ";
            value &= ~(1 << 14);
            printf("%04X", value);
        }
        else{
            cout << "-";
            value = (value - 1) ^ 0b11111111111111;
            value &= ~(1 << 14);
            printf("%04X", value);
        }
    }
}

unsigned long bigchars[18] = {
        0b1111111111000011110000111100001111000011110000111100001111111111,
        0b1111111100111000001110000011100000111000001100110011011000111100,
        0b1111111111000111000011100001110000111000011100001110001111111111,
        0b1111111111100011111000001111111111111111111000001110001111111111,
        0b1110000011100000111000001111111111100111111001111110011011100110,
        0b1111111111100011111000001111111111111111000001111100011111111111,
        0b1111111111100011111000111110001101111111000001111100011111111111,
        0b0000011100000110000011100001110000111000111000001100001111111111,
        0b1111111111000011110000110011110000111100110000111100001111111111,
        0b1111111111000011110000001111111011000111110001111100011111111110,
        0b1110001111000011110000111111111111000011110000110110011000111100,
        0b0111111111000011110000110111111101111111110000111100001101111111,
        0b1111111111000011110000110000001100000011110000111100001111111111,
        0b0011111101110001110000011100000111000001110000010111000100111111,
        0b1111111101111111000001111111111111111111000001110111111111111111,
        0b0000001100000011000000110001111100000011000000110011111111111111,
        0b0000000000000000000110001111111100011000000000000000000000000000,
        0b0000000000000000000000001111111100000000000000000000000000000000
};

void drawbigchar(int num) {
    short value;
    sc_memoryGet(num, value);


    if (((value >> 14) & 1) == 0) {  // команда
        bc_printbigchar(bigchars[16], 2, 14, WHITE, CYAN);
        for (int j = 0; j < 4; j++) {
            int ch = (value & (0xF << (4 * (3 - j)))) >> (4 * (3 - j));
            bc_printbigchar(bigchars[ch], 11 + 9 * j, 14, WHITE, CYAN);
        }
    }
    else{
        if (((value >> 13) & 1) == 0) {
            value &= ~(1 << 14);
            for (int j = 0; j < 4; j++) {
                int ch = (value & (0xF << (4 * (3 - j)))) >> (4 * (3 - j));
                bc_printbigchar(bigchars[ch], 11 + 9 * j, 14, WHITE, CYAN);
            }
        }
        else{
            bc_printbigchar(bigchars[17], 2, 14, WHITE, CYAN);
            value = (value - 1) ^ 0b11111111111111;
            value &= ~(1 << 14);
            for (int j = 0; j < 4; j++) {
                int ch = (value & (0xF << (4 * (3 - j)))) >> (4 * (3 - j));
                bc_printbigchar(bigchars[ch], 11 + 9 * j, 14, WHITE, CYAN);
            }
        }
    }
}

void drawoper(short thiscell){
    short value;
    mt_gotoXY(68, 8);
    sc_memoryGet(thiscell, value);
    if (((value >> 14) & 1) == 0){
        short operand;
        short command;
        sc_commandDecode(value, command, operand);
        cout << hex << setfill('0') << setw(2) << uppercase << command << " : " << setfill('0') << setw(2) << uppercase << operand;
        cout << dec;
    }else{
        cout << "no command";
    }
}

int main() {

    rk_mytermsave();
    short thiscell = 0;
    KEYS clicked_key;
    sc_regSet(SC_IGNORE_CLOCK, 1);
    sc_memoryInit();
    short change_value;
    regex pos(R"([0-9A-Fa-f]{1,4})");
    regex neg(R"([-]{1}[0-9A-Fa-f]{1,4})");
    regex com(R"([+]{1}[0-9A-Fa-f]{1,4})");
    signal(SIGALRM, runhandler);
    signal(SIGUSR1, resetHandler);
    while (true) {
        drawmenu();
        drawflags();
        mt_gotoXY(70, 5);
        printf("%04X", instructionCounter);
        mt_gotoXY(70, 2);
        drawaccum();
        mt_gotoXY(2, 2);
        for (int i = 0; i < 100; i++) {
            if (i == thiscell) {
                mt_setbgcolor(CYAN);
            }
            drawcell(i);
            if (i % 10 == 9)
                mt_gotoXY(2, 2 + (i / 9));
            else
                cout << " ";
            if (i == thiscell) {
                printf("\E[0m");
            }
        }
        drawbigchar(thiscell);
        drawoper(thiscell);
        mt_gotoXY(12, 24);
        fflush(stdin);
        fflush(stdout);
        rk_readkey(&clicked_key);
        string enter_value;
        string accum;
        switch (clicked_key) {
            case UP_ARROW_KEY:
                (thiscell <= 9) ? (thiscell = 90 + thiscell) : (thiscell -= 10);
                break;
            case RIGHT_ARROW_KEY:
                (!((thiscell + 1) % 10)) ? (thiscell -= 9) : (thiscell += 1);
                break;
            case DOWN_ARROW_KEY:
                (thiscell >= 90) ? (thiscell = thiscell - 90) : (thiscell += 10);
                break;
            case LEFT_ARROW_KEY:
                (!(thiscell % 10)) ? (thiscell += 9) : (thiscell -= 1);
                break;
            case ESC_KEY:
                return 0;
            case ENTER_KEY:
                cout << "Enter value: ";
                cin >> enter_value;
                if(regex_match(enter_value, pos)){
                    change_value = (short) stoi(enter_value, nullptr, 16);
                    change_value |= (1 << 14);
                    sc_memorySet(thiscell, change_value);
                }
                else if(regex_match(enter_value, neg)){
                    change_value =  stoi(enter_value, nullptr, 16);
                    change_value = ~(change_value - 1);//стало просто числом
                    change_value |= (1 << 14); // стало не командой внутри системы СК
                    change_value ^= 0b11111111111111;
                    change_value += 1;
                    sc_memorySet(thiscell, change_value);
                }
                else if(regex_match(enter_value, com)){
                    change_value = (short) stoi(enter_value, nullptr, 16);
                    short temp;
                    if(sc_commandEncode(((change_value >> 8) & 0xFF), (change_value & 0xFF),temp) == -1) {
                        sc_regSet(SC_WRONG_COMMAND, 1);
                        return -1;
                    }
                    sc_memorySet(thiscell, temp);
                }else{
                    return -1;
                }
                break;
            case F5_KEY:
                cout << "Enter value: ";
                cin >> accum;
                if(regex_match(accum, pos)){
                    accumulator = (short) stoi(accum, nullptr, 16);
                    accumulator |= (1 << 14);
                }
                else if(regex_match(accum, neg)){
                    accumulator =  stoi(accum, nullptr, 16);
                    accumulator = ~(accumulator - 1);//стало просто числом
                    accumulator |= (1 << 14); // стало не командой внутри системы СК
                    accumulator ^= 0b11111111111111;
                    accumulator += 1;
                }else{
                    return -1;
                }
                break;
            case F6_KEY:
                cout << "Enter value: ";
                cin >> instructionCounter;
                break;
            case L_KEY:
                cout << "Enter filename: ";
                char load_name[100];
                cin >> load_name;
                sc_memoryLoad(load_name);
                break;
            case S_KEY:
                cout << "Enter filename: ";
                char save_name[100];
                cin >> save_name;
                sc_memorySave(save_name);
                break;
            case R_KEY:
                sc_regSet(SC_IGNORE_CLOCK, 0);
                raise(SIGALRM);
                break;
            case T_KEY:
                CU();
                break;
            case I_KEY:
                raise(SIGUSR1);
                break;
            case Q_KEY:
                break;
        }
    }
}


