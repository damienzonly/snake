#ifndef __SNAKE_DS
#define __SNAKE_DS
#include <termios.h>

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} MOVE;

typedef struct Segment {
    uint16_t x;
    uint16_t y;
    struct Segment* next;
} Segment;

typedef struct {
    Segment* head;
    uint16_t length;
} Snake;

typedef struct {
    Snake* snake;
    uint16_t board_width;
    uint16_t board_height;
    MOVE direction;
    uint32_t speed;
    pthread_mutex_t mtx;
    uint8_t dead;
    uint8_t apple_x;
    uint8_t apple_y;
    uint32_t score;
} GameObjects;

#endif