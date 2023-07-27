#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"
#include "snake.h"

void draw(int boardW, int boardY, Snake* snake) {
    int matrix[boardW][boardY];
    for (int y = 0; y < boardY; ++y) {
        for (int x = 0; x < boardW; ++x) {
            if (y == 0 || y == boardY-1) {
                matrix[x][y] = '#';
            } else {
                if (x == 0 || x == boardW-1) {
                    matrix[x][y] = '#';
                } else {
                    matrix[x][y] = ' ';
                }
            }
        }
    }
    Segment* s = snake->head;
    while (s->next != NULL) {
        matrix[s->x][s->y] = 'o';
        s = s->next;
    }
    for (int y = 0; y < boardY; ++y) {
        for (int x = 0; x < boardW; ++x) {
            printf("%c", matrix[x][y]);
        }
        printf("\n");
    }
}

void t_user_input(void* data) {
    Snake* snake = (Snake*) data;
    while (1) {
        int c = getchar();
        switch(c) {
            case 'a':
            case 'h':
                move(snake, LEFT);
                break;
            case 's':
            case 'j':
                move(snake, DOWN);
                break;
            case 'd':
            case 'l':
                move(snake, RIGHT);
                break;
            case 'w':
            case 'k':
                move(snake, UP);
                break;
        }
    }
}

Segment* _create_segment(int x, int y) {
    Segment* pSegment = (Segment*)malloc(sizeof(Segment));
    pSegment->next = NULL;
    pSegment->x = x;
    pSegment->y = y;
    return pSegment;
}

void add_segment(Snake* snake, int x, int y) {
    Segment* s = snake->head;
    Segment* new_segment = _create_segment(x, y);
    if (s == NULL) {
        snake->head = new_segment;
        ++snake->length;
        return;
    } else while (s->next != NULL) {
        s = s->next;
    }
    s->next = new_segment;
    snake->length++;
    return;
}

void add_segment_front(Snake* snake, int x, int y) {
    Segment* pHead = snake->head;
    Segment* newSegment = _create_segment(x, y);
    snake->length++;
    snake->head = newSegment;
    snake->head->next = pHead;
}

void move(Snake* snake, MOVE m) {
    if (m == UP) add_segment_front(snake, snake->head->x, snake->head->y-1);
    else if (m == DOWN) add_segment_front(snake, snake->head->x, snake->head->y+1);
    else if (m == LEFT) add_segment_front(snake, snake->head->x-1, snake->head->y);
    else if (m == RIGHT) add_segment_front(snake, snake->head->x+1, snake->head->y);
}

void print_snake(Snake* snake) {
    Segment* s = snake->head;
    while (s->next != NULL) {
        printf("(%d,%d) ", s->x, s->y);
        s = s->next;
    }
    printf("NULL\n");
}