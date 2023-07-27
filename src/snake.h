#ifndef __SNAKE
#define __SNAKE
#include "data_structures.h"

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} MOVE;
void draw(int, int, Snake*);
void t_user_input(void* data);
void print_snake(Snake*);
void move(Snake* snake, MOVE direction);
Segment* _create_segment(int x, int y);
void add_segment_tail(Snake* snake, int x, int y);
void add_segment_head(Snake* snake, int x, int y);

#endif