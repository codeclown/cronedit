#ifndef TERMINAL_H
#define TERMINAL_H

bool is_tty();

int get_terminal_width();

int get_terminal_height();

void init_terminal();

int terminal_get_char();

void revert_terminal();

void prepare_for_input();

int KEY_UP = 65;
int KEY_RIGHT = 67;
int KEY_DOWN = 66;
int KEY_LEFT = 68;

int KEY_Q = 113;
int KEY_ESC = 27;
int KEY_ENTER = 10;
int KEY_C = 99;
int KEY_N = 110;
int KEY_D = 100;
int KEY_R = 114;
int KEY_S = 115;

#endif
