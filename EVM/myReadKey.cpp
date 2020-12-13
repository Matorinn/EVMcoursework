//
// Created by mato on 27.05.2020.
//

#include "myReadKey.h"
#include <termios.h>
#include <unistd.h>
#include <cstring>


termios terminal_save;

void rk_mytermsave() {
    tcgetattr(STDIN_FILENO, &terminal_save);
}

void rk_mytermrestore() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal_save);
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint) {
    termios myregime;
    if (regime == 0) {
        myregime.c_lflag |= ICANON;
    } else if (regime == 1) {
        myregime.c_lflag &= ~ICANON;
        echo == 0 ? myregime.c_lflag &= ~ECHO : myregime.c_lflag |= ECHO;
        sigint == 0 ? myregime.c_lflag &= ~ISIG : myregime.c_lflag |= ISIG;
        myregime.c_cc[VMIN] = vmin;
        myregime.c_cc[VTIME] = vtime;
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &myregime);
    return 0;
}

int rk_readkey(KEYS *key) {
    char buffer[16];
    rk_mytermregime(1, 1, 0, 0, 1);

    int ch = read(STDIN_FILENO, buffer, 16);
    buffer[ch] = '\0';
    if (strcmp(buffer, "\E[D") == 0)
        *key = KEYS::LEFT_ARROW_KEY;
    else if (strcmp(buffer, "\E[C") == 0)
        *key = KEYS::RIGHT_ARROW_KEY;
    else if (strcmp(buffer, "\E[A") == 0)
        *key = KEYS::UP_ARROW_KEY;
    else if (strcmp(buffer, "\E[B") == 0)
        *key = KEYS::DOWN_ARROW_KEY;
    else if (strcmp(buffer, "\n") == 0)
        *key = KEYS::ENTER_KEY;
    else if (strcmp(buffer, "\E") == 0)
        *key = KEYS::ESC_KEY;
    else if (strcmp(buffer, "\E[15~") == 0)
        *key = KEYS::F5_KEY;
    else if (strcmp(buffer, "\E[17~") == 0)
        *key = KEYS::F6_KEY;
    else if (strcmp(buffer, "l") == 0)
        *key = KEYS::L_KEY;
    else if (strcmp(buffer, "s") == 0)
        *key = KEYS::S_KEY;
    else if (strcmp(buffer, "r") == 0)
        *key = KEYS::R_KEY;
    else if (strcmp(buffer, "t") == 0)
        *key = KEYS::T_KEY;
    else if (strcmp(buffer, "i") == 0)
        *key = KEYS::I_KEY;
    else if (strcmp(buffer, "q") == 0)
        *key = KEYS::Q_KEY;
    else
        *key = KEYS::NON_KEY;
    rk_mytermrestore();
    return 0;
}

//key_f5=\E[15~
//key_f6=\E[17~
//key_enter=\EOM
//key_left=\EOD
//key_right=\EOC
//key_down=\EOB
//key_up=\EOA

