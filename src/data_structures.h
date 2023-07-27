#ifndef __SNAKE_DS
#define __SNAKE_DS

typedef struct {
    int x;
    int y;
} Coordinates;

typedef struct {
    int length;
    Coordinates segments[];
} Snake;

#endif