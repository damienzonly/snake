#ifndef __SNAKE_DS
#define __SNAKE_DS
#include <termios.h>

typedef struct Segment {
    int x;
    int y;
    struct Segment* next;
} Segment;

typedef struct {
    Segment* head;
    int length;
} Snake;

typedef struct {
    Snake* snake;
    int board_width;
    int board_height;
} GameObjects;

#endif