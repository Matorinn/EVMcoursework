//
// Created by mato on 23.05.2020.
//

#ifndef EVM_MYBIGCHARS_H
#define EVM_MYBIGCHARS_H

#include "myTermColors.h"
#include "myTerm.h"

int bc_printA(char *str);

int bc_box(int x1, int y1, int x2, int y2);

int bc_printbigchar(long storage, int x, int y, MY_TERM_COLORS color, MY_TERM_COLORS color2);

int bc_setbigcharpos(long storage, int x, int y, int value);

int bc_getbigcharpos(long storage, int x, int y, int &value);

int bc_bigcharwrite(char *filename, unsigned long *storage, int count);

int bc_bigcharread(char *filename, unsigned long *storage, int need_count);

#endif //EVM_MYBIGCHARS_H
