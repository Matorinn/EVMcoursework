//
// Created by mato on 23.05.2020.
//
#include <iostream>
#include "myBigChars.h"

#define BC_LEFT_UP (char*) "l"
#define BC_LEFT_DOWN (char*) "m"
#define BC_RIGHT_UP (char*) "k"
#define BC_RIGHT_DOWN (char*) "j"
#define BC_SIDE_H (char*) "q"
#define BC_SIDE_V (char*) "x"
#define BC_SPACE (char*) " "
#define BC_BLOCK (char*) "a"


int bc_printA(char *str) {
    printf("\E(0%s\E(B", str);
    return 0;
}

int bc_box(int x1, int y1, int x2, int y2) {
    mt_gotoXY(x1, y1);
    for (int i = 0; i < x2; i++) {
        if (i == 0)
            bc_printA(BC_LEFT_UP);
        else if (i == x2 - 1)
            bc_printA(BC_RIGHT_UP);
        else
            bc_printA(BC_SIDE_H);
    }
    for (int i = 0; i < (y2 - 2); i++) {
        //mt_gotoXY(x1 + x2 - 1, y1 + i);
        mt_gotoXY(x1 + x2 - 1, y1 + (i + 1));
        bc_printA(BC_SIDE_V);
    }
    mt_gotoXY(x1, y1 + y2 - 1);
    for (int i = 0; i < x2; i++) {
        if (i == 0)
            bc_printA(BC_LEFT_DOWN);
        else if (i == x2 - 1)
            bc_printA(BC_RIGHT_DOWN);
        else
            bc_printA(BC_SIDE_H);
    }
    for (int i = 0; i < y2 - 2; i++) {
        mt_gotoXY(x1, y1 + (i + 1));
        bc_printA(BC_SIDE_V);
    }

    return 0;
}

int bc_printbigchar(long storage, int x, int y, MY_TERM_COLORS color, MY_TERM_COLORS color2) {
    mt_gotoXY(x, y);
    mt_setfgcolor(color);
    mt_setbgcolor(color2);
    unsigned long flag;
    for (int i = 0; i < 64; i++) {
        if (i == 0) {
            flag = storage;
            flag &= 1;
            flag == 0 ? bc_printA(BC_SPACE) : bc_printA(BC_BLOCK);
        } else {
            flag = (storage >> i);
            flag &= 1;
            flag == 0 ? bc_printA(BC_SPACE) : bc_printA(BC_BLOCK);
        }
        if (i % 8 == 7)
            mt_gotoXY(x, y + i / 7);
    }
    printf("\E[0m");
    return 0;
}

int bc_setbigcharpos(long storage, int x, int y, int value) {
    if (value != 1 && value != 0)
        return -1;
    int shift = y * 8 + x;
    value == 0 ? storage &= ~(1L << shift) : storage |= (1L << shift);
    std::cout << storage;
    return 0;
}

int bc_getbigcharpos(long storage, int x, int y, int &value) {
    int shift = y * 8 + x;
    value = (storage >> shift) & 1;

    return 0;
}

int bc_bigcharwrite(char *filename, unsigned long *storage, int count) {
    FILE *fp;
    if ((fp = fopen(filename, "w+")) == NULL) {
        printf("Cannot open file.");
    }
    fwrite(storage, sizeof(unsigned long), count, fp);
    fclose(fp);
    return 0;
}

int bc_bigcharread(char *filename, unsigned long *storage, int need_count) {
    FILE *fp;
    if ((fp = fopen(filename, "r+")) == NULL) {
        printf("Cannot open file.");
    }
    fread(storage, sizeof(unsigned long), need_count, fp);
    fclose(fp);
    return 0;
}