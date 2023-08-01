#ifndef __SNAKE
#define __SNAKE
#include "data_structures.h"

void draw(uint16_t, uint16_t, Snake*, GameObjects* game);
void t_user_input(void* data);
void print_snake(Snake*);
void move(Snake* snake, MOVE direction);
Segment* _create_segment(uint16_t x, uint16_t y);
void add_segment_tail(Snake* snake, uint16_t x, uint16_t y);
void add_segment_head(Snake* snake, uint16_t x, uint16_t y);
uint16_t is_collision(Snake* snake, uint16_t next_x, uint16_t next_y, uint16_t board_width, uint16_t board_height);
uint16_t segment_exists(Segment* head, uint16_t x, uint16_t y);
uint16_t is_collision(Snake* snake, uint16_t next_x, uint16_t next_y, uint16_t board_width, uint16_t board_height);
void free_snake(Snake* snake);
void clear_screen();
MOVE parse_direction(char c, uint16_t*, uint16_t*);
int board_rand(int min, int max);
void print_banner(GameObjects*);
void pop_segment(Snake* snake);
#endif