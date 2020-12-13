//
// Created by mato on 23.05.2020.
//

#include "myTerm.h"
#include <sys/ioctl.h>
#include <cstdio>

void mt_clrscr() {
    printf("\E[H\E[2J");
}

int mt_gotoXY(int x, int y) {
    printf("\E[%d;%dH", y, x);
    return 0;
}

//TODO:добавить проверку размера размера экрана?
int mt_getscreensize(int *rows, int *cols) {
    winsize ws;

    if (ioctl(1, TIOCGWINSZ, &ws) != 0) {
        return 1;
    }
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
}

void mt_setfgcolor(MY_TERM_COLORS color) {
    printf("\E[3%dm", color);
}

void mt_setbgcolor(MY_TERM_COLORS color) {
    printf("\E[4%dm", color);
}
