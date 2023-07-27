#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include "snake.h"
#include "data_structures.h"

size_t board_width;
size_t board_height;
size_t dead;

struct termios term_opts;

void clear_screen() {
    printf("\033[2J\033[H"); // ANSI escape code to clear the screen and move cursor to (0,0)
}

int main() {
    dead = 0;
    board_width = 30;
    board_height = board_width / 2;
    int length = 3;
    int initialX = board_width*0.3;
    int initialY = board_height*0.3;

    // initialize termios structs
    tcgetattr(0, &term_opts);
    term_opts.c_lflag &= ~ICANON; // disable canonical mode, e.g. char by char input instead of line by line
    term_opts.c_lflag &= ~ECHO; // disable echoing typed characters to output
    tcsetattr(0, TCSANOW, &term_opts); // apply 
    // initialize snake segments and length
    Snake* snake = (Snake*)malloc(sizeof(Snake) + sizeof(Segment) * length);
    snake->length = length;
    for (int i = 0; i < 5; ++i) {
        add_segment_tail(snake, initialX + i, initialY);
    }
    pthread_t t_input;
    pthread_create(&t_input, NULL, (void*) t_user_input, snake);
    while (!dead) {
        clear_screen();
        // print_snake(snake);
        draw(board_width, board_height, snake);
        usleep(100000);
    }
    pthread_join(t_input, NULL);
    free(snake);
    
    return 0;
}