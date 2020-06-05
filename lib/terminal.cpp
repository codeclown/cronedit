#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#define HIDE_CURSOR   "\e[?25l"
#define SHOW_CURSOR   "\e[?25h"
#define BEGINNING_OF_PREVIOUS_LINE   "\033[G\033[1A"

bool is_tty() {
  return isatty(fileno(stdin));
}

struct winsize win_size;
int get_terminal_width() {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size);
  return win_size.ws_col;
}
int get_terminal_height() {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size);
  return win_size.ws_row;
}

// Did not have to bring in ncurses, thanks to this answer:
// https://stackoverflow.com/a/12710738/239527

struct termios oldt, newt;

void init_terminal() {
  tcgetattr(fileno(stdin), &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(fileno(stdin), TCSANOW, &newt);
  std::cout << HIDE_CURSOR;
}

int terminal_get_char() {
  return getchar();
}

void revert_terminal() {
  std::cout << SHOW_CURSOR;
  tcsetattr(fileno(stdin), TCSANOW, &oldt);
}

void prepare_for_input() {
  std::cout << BEGINNING_OF_PREVIOUS_LINE;
}
