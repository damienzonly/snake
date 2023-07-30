#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"
#include "snake.h"
#include "tty.h"


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

int check_collision(Snake* snake, int nextX, int nextY, int board_width, int board_height) {
    Segment* seg = snake->head;
    if (segment_exists(seg->next, nextX, nextY)) return 1;
    while (seg->next != NULL) {
        seg = seg->next;
        if (
            nextX == 0 || nextY == 0 ||
            nextX == board_width-1 || nextY == board_height-1
        ) return 1;
    }
    return 0;
}

int segment_exists(Segment* head, int x, int y) {
    while (head->next != NULL) {
        head = head->next;
        if (head->x == x && head->y == y) return 1;
    }
    return 0;
}

void t_user_input(void* data) {
    GameObjects* go = (GameObjects*)data;
    Snake* snake = go->snake;
    int nextX = snake->head->x, nextY = snake->head->y;
    MOVE direction;
    while (1) {
        set_termios_opts();
        int c = getchar();
        unset_termios_opts();
        switch(c) {
            case 'a':
            case 'h':
                nextX = snake->head->x-1;
                direction = LEFT;
                break;
            case 's':
            case 'j':
                nextY = snake->head->y+1;
                direction = DOWN;
                break;
            case 'd':
            case 'l':
                nextX = snake->head->x+1;
                direction = RIGHT;
                break;
            case 'w':
            case 'k':
                nextY = snake->head->y-1;
                direction = UP;
                break;
        }
        if (!check_collision(snake, nextX, nextY, go->board_width, go->board_height))
            move(snake, direction);
        else {
            printf("game over\n");
            exit(1);
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

void add_segment_tail(Snake* snake, int x, int y) {
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

void add_segment_head(Snake* snake, int x, int y) {
    Segment* pHead = snake->head;
    Segment* newSegment = _create_segment(x, y);
    snake->head = newSegment;
    snake->head->next = pHead;
    snake->length++;
}

// todo: when less than 3 segments, may be a problem
void pop_segment(Snake* snake) {
    Segment* s = snake->head;
    while (s->next->next != NULL) {
        s = s->next;
    }
    free(s->next);
    s->next = NULL;
    snake->length--;
}

void move(Snake* snake, MOVE m) {
    int x = snake->head->x, y = snake->head->y;
    if (m == UP) add_segment_head(snake, x, y-1);
    else if (m == DOWN) add_segment_head(snake, x, y+1);
    else if (m == LEFT) add_segment_head(snake, x-1, y);
    else if (m == RIGHT) add_segment_head(snake, x+1, y);
    else return;
    pop_segment(snake);
}

void print_snake(Snake* snake) {
    Segment* s = snake->head;
    while (s->next != NULL) {
        printf("(%d,%d) ", s->x, s->y);
        s = s->next;
    }
    printf("NULL\n");
}