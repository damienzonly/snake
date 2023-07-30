#include <termios.h>


void set_termios_opts() {
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(0, TCSANOW, &t);
}

void unset_termios_opts() {
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= (ECHO | ICANON);
    tcsetattr(0, TCSANOW, &t);
}\

