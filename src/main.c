#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include "snake.h"
#include "data_structures.h"
#include "string.h"
#include "tty.h"

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
    srand(time(0));
    uint16_t board_width, board_height,
            length, initialX, initialY;
    pthread_t thread;
    // signal(SIGINT, signal_handler);
    // signal(SIGTERM, signal_handler);
    board_width = 50;
    board_height = board_width / 2;
    length = 5;
    initialX = board_width*0.3;
    initialY = board_height*0.3;

    // initialize snake segments and length
    Snake* snake = (Snake*)malloc(sizeof(Snake) + sizeof(Segment) * length);
    snake->length = length;
    for (int i = 0; i <= length; ++i) {
        add_segment_tail(snake, initialX + i, initialY);
    }
    
    GameObjects game = {
        .board_width = board_width,
        .board_height = board_height,
        .snake = snake,
        .direction = UP,
        .speed = 100000,
        .dead = 0,
        .score = 0,
        .apple_x = board_rand(1, board_width),
        .apple_y = board_rand(1, board_height)
    };

    if ((pthread_mutex_init(&game.mtx, NULL)) != 0) {
        printf("failed initializing mutex\n");
        exit(1);
    }
    
    // first frame
    int pid = getpid();
    printf("pid: %d\n", pid);
    draw(board_width, board_height, snake, &game);
    set_termios_opts();
    game.direction = parse_direction(getchar(), NULL, NULL);;
    pthread_create(&thread, NULL, (void*) t_user_input, &game);
    while (1) {
        printf("pid: %d\n", pid);
        pthread_mutex_lock(&game.mtx);
        if (game.dead) break;
        move(snake, game.direction);
        draw(board_width, board_height, snake, &game);
        print_snake(snake);
        printf("SCORE: %d\n", game.score);
        printf("[dead]: %d\n", game.dead);
        printf("[direction]: %d\n", game.direction);
        printf("[bw/bh]: %dx%d\n", board_width, board_height);
        pthread_mutex_unlock(&game.mtx);
        usleep(game.speed);
    }
    printf("game over\n");
    pthread_join(thread, NULL);
    // free_snake(snake);
    return 0;
}