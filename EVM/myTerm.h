//
// Created by mato on 23.05.2020.
//

#ifndef EVMLABS_MYTERM_H
#define EVMLABS_MYTERM_H

#include "myTermColors.h"

void mt_clrscr();

int mt_gotoXY(int x, int y);

int mt_getscreensize(int *rows, int *cols);

void mt_setfgcolor(MY_TERM_COLORS color);

void mt_setbgcolor(MY_TERM_COLORS color);

#endif //EVMLABS_MYTERM_H
