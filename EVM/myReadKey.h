//
// Created by mato on 27.05.2020.
//

#ifndef EVM_MYREADKEY_H
#define EVM_MYREADKEY_H

#include "keys.h"

int rk_readkey (KEYS *key);
void rk_mytermsave ();
void rk_mytermrestore ();
int rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint);

#endif //EVM_MYREADKEY_H
