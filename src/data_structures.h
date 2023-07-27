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

void add_segment(Snake*, int, int);

#endif