#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "data_structures.h"
#include "snake.h"
#include "tty.h"

void clear_screen() {
    printf("\033[2J\033[H"); // ANSI escape code to clear the screen and move cursor to (0,0)
}

void draw(uint16_t boardW, uint16_t boardY, Snake* snake) {
    clear_screen();
    uint16_t matrix[boardW][boardY];
    for (int y = 0; y < boardY; ++y) {
        for (uint16_t x = 0; x < boardW; ++x) {
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
    for (uint16_t y = 0; y < boardY; ++y) {
        for (uint16_t x = 0; x < boardW; ++x) {
            printf("%c", matrix[x][y]);
        }
        printf("\n");
    }
}

uint16_t check_collision(Snake* snake, uint16_t nextX, uint16_t nextY, uint16_t board_width, uint16_t board_height) {
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

uint16_t segment_exists(Segment* head, uint16_t x, uint16_t y) {
    while (head->next != NULL) {
        head = head->next;
        if (head->x == x && head->y == y) return 1;
    }
    return 0;
}

MOVE char_to_direction(char c) {
    MOVE direction;
    switch(c) {
        case 'a':
        case 'h':
            direction = LEFT;
            break;
        case 's':
        case 'j':
            direction = DOWN;
            break;
        case 'd':
        case 'l':
            direction = RIGHT;
            break;
        case 'w':
        case 'k':
            direction = UP;
            break;
        default:
            direction = RIGHT;
    }
    return direction;
}

void t_user_input(void* data) {
    GameObjects* go = (GameObjects*)data;
    Snake* snake = go->snake;
    uint16_t nextX = snake->head->x, nextY = snake->head->y;
    while (1) {
        uint16_t c = getchar();
        pthread_mutex_lock(&go->mtx);
        go->direction = char_to_direction(c);
        if (check_collision(snake, nextX, nextY, go->board_width, go->board_height)) {
            go->dead = 1;
            break;
        }
        pthread_mutex_unlock(&go->mtx);
    }
}

Segment* _create_segment(uint16_t x, uint16_t y) {
    Segment* pSegment = (Segment*)malloc(sizeof(Segment));
    pSegment->next = NULL;
    pSegment->x = x;
    pSegment->y = y;
    return pSegment;
}

void add_segment_tail(Snake* snake, uint16_t x, uint16_t y) {
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

void add_segment_head(Snake* snake, uint16_t x, uint16_t y) {
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

void free_snake(Snake* snake) {
    Segment* s = snake->head;
    while (s->next != NULL) {
        free(s);
        s = s->next;
    }
    free(snake);
}