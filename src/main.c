#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "snake.h"
#include "data_structures.h"

size_t board_width;
size_t board_height;
size_t dead;

int main() {
    dead = 0;
    board_width = 30;
    board_height = board_width / 2;
    
    int length = 3;
    Coordinates c[] = {
        {board_width*0.2, 0},
        {board_width*0.2, 1},
        {board_width*0.2, 2}        
    };
    Snake* snake = (Snake*)malloc(sizeof(Snake) + sizeof(Coordinates) * length);
    snake->length = length;
    for (int i=0; i < snake->length; ++i) {
        snake->segments[i] = c[i];
    }
    draw(board_width, board_height, snake);
    free(snake);
    return 0;
}