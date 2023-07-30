#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include "snake.h"
#include "data_structures.h"
#include "string.h"

size_t board_width;
size_t board_height;
size_t dead;

void clear_screen() {
    printf("\033[2J\033[H"); // ANSI escape code to clear the screen and move cursor to (0,0)
}

void signal_handler(int signal) {
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ICANON;
    t.c_lflag &= ECHO;
    tcsetattr(0, TCSANOW, &t);
    char* message = "message\n";
    write(0, message, strlen(message));
}

int main() {
    // signal(SIGINT, signal_handler);
    // signal(SIGTERM, signal_handler);
    dead = 0;
    board_width = 20;
    board_height = board_width / 2;
    int length = 5;
    int initialX = board_width*0.3;
    int initialY = board_height*0.3;

    // initialize snake segments and length
    Snake* snake = (Snake*)malloc(sizeof(Snake) + sizeof(Segment) * length);
    snake->length = length;
    for (int i = 0; i <= length; ++i) {
        add_segment_tail(snake, initialX + i, initialY);
    }
    pthread_t t_input;
    GameObjects game_obj = {
        .board_width = board_width,
        .board_height = board_height,
        .snake = snake
    };
    pthread_create(&t_input, NULL, (void*) t_user_input, &game_obj);
    while (!dead) {
        clear_screen();
        printf("pid: %d\n", getpid());
        draw(board_width, board_height, snake);
        usleep(100000);
    }
    pthread_join(t_input, NULL);
    free(snake);
    
    return 0;
}