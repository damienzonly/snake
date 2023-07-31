#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "data_structures.h"
#include "snake.h"
#include "tty.h"

void clear_screen() {
    printf("\033[2J\033[H"); // ANSI escape code to clear the screen and move cursor to (0,0)
}

void draw(uint16_t boardW, uint16_t boardY, Snake* snake, GameObjects* game) {
    clear_screen();
    uint16_t matrix[boardW][boardY];
    for (int y = 0; y < boardY; ++y) {
        for (uint16_t x = 0; x < boardW; ++x) {
            // apple
            if (x == game->apple_x && y == game->apple_y) {
                matrix[x][y] = 'X';
            }
            // wall
            else if (y == 0 || y == boardY-1 || x == 0 || x == boardW-1) {
                matrix[x][y] = '#';
            }
            // space
            else {
                matrix[x][y] = ' ';
            }
        }
    }
    Segment* s = snake->head;
    char head;
    MOVE d = game->direction;
    if (d == UP) head = '^';
    if (d == DOWN) head = 'V';
    if (d == LEFT) head = '<';
    if (d == RIGHT) head = '>';
    matrix[s->x][s->y] = head;
    while (s->next != NULL) {
        matrix[s->next->x][s->next->y] = 'o';
        s = s->next;
    }
    for (uint16_t y = 0; y < boardY; ++y) {
        for (uint16_t x = 0; x < boardW; ++x) {
            printf("%c", matrix[x][y]);
        }
        printf("\n");
    }
}

uint16_t is_collision(Snake* snake, uint16_t next_x, uint16_t next_y, uint16_t board_width, uint16_t board_height) {
    Segment* seg = snake->head;
    // exclude the head for the search
    return (
        segment_exists(seg->next, next_x, next_y) ||
        next_x == 0 || next_y == 0 ||
        next_x == board_width-1 || next_y == board_height-1
    );
}

uint16_t segment_exists(Segment* head, uint16_t x, uint16_t y) {
    while (head->next != NULL) {
        if (head->x == x && head->y == y) return 1;
        head = head->next;
    }
    return 0;
}

MOVE parse_direction(char c, uint16_t* next_x, uint16_t* next_y) {
    MOVE direction;
    switch(c) {
        case 'a':
        case 'h':
            direction = LEFT;
            if (next_x != NULL) *next_x -= 1;
            break;
        case 's':
        case 'j':
            direction = DOWN;
            if (next_y != NULL) *next_y += 1;
            break;
        case 'd':
        case 'l':
            direction = RIGHT;
            if (next_x != NULL) *next_x += 1;
            break;
        case 'w':
        case 'k':
            direction = UP;
            if (next_y != NULL) *next_y -= 1;
            break;
        default:
            direction = -1;
    }
    return direction;
}

void t_user_input(void* data) {
    GameObjects* game = (GameObjects*)data;
    Snake* snake = game->snake;
    while (1) {
        uint16_t c = getchar();
        pthread_mutex_lock(&game->mtx);
        uint16_t next_x = snake->head->x, next_y = snake->head->y;
        MOVE next_move = parse_direction(c, &next_x, &next_y);
        uint8_t direction_sum = game->direction + next_move;
        // if moving in opposite direction respect to the current one
        // UP + DOWN = 1
        // LEFT + RIGHT = 5
        if (direction_sum == 1 || direction_sum == 5) {
            pthread_mutex_unlock(&game->mtx);
            continue;
        }
        game->direction = next_move;
        // fprintf(stderr, "nx: %d, ny: %d\n", next_x, next_y);
        if (is_collision(snake, next_x, next_y, game->board_width, game->board_height)) {
            game->dead = 1;
            fprintf(stderr, ">>>(%d, %d)\n", next_x, next_y);
            pthread_mutex_unlock(&game->mtx);
            break;
        }
        if (game->apple_x == next_x && game->apple_y == next_y) {
            game->apple_x = board_rand(1, game->board_width);
            game->apple_y = board_rand(1, game->board_height);
            game->score += 100;
            game->speed += 50000;
        }
        pthread_mutex_unlock(&game->mtx);
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

uint16_t board_rand(uint16_t min, uint16_t max) {
   return (rand() % (max - min + 1)) + min;
}