#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define COLS 10
#define ROWS 5
#define WIDTH ROWS+2 // rows
#define HEIGHT COLS+2 // columns
#define SNAKE_LENGTH (WIDTH-2)*(HEIGHT-2)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

int msleep(long tms)
{
    struct timespec ts;
    int ret;
    if (tms < 0)
    {
        errno = EINVAL;
        return -1;
    }
    ts.tv_sec = tms / 1000;
    ts.tv_nsec = (tms % 1000) * 1000000;
    do {
        ret = nanosleep(&ts, &ts);
    } while (ret && errno == EINTR);
    return ret;
}

int width, height, max_snake_length;

int score = 0;
int snake_length = 1;


void init() {
	width = 30;
	height = 60;
	max_snake_length = ((width-3) * (height-2)) - 1; // width minus borders minus newfile char, height minus borders, minus 1 for 0-based indexes
}

void draw_board(int board[WIDTH][HEIGHT]) {
    msleep(1000);
    system("clear");
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

void set_board(int board[WIDTH][HEIGHT]) {
    for (int w = 0; w < WIDTH; ++w) {
        for (int h = 0; h < HEIGHT; ++h) {
            if (w == 0) board[w][h] = '_';
            else if (w == WIDTH-1) board[w][h] = '-';
            else if (h == 0 || h == HEIGHT-1) board[w][h] = '|';
            else board[w][h] = '~';
        }
    }
}

void apply_snake_coords(int snake[SNAKE_LENGTH][2], int board[WIDTH][HEIGHT]) {
    for (int r = 0; r < SNAKE_LENGTH-1; ++r) {
        int x = snake[r][0];
        int y = snake[r][1];
        if (x + y == 0) return;
        board[x][y] = 'X';
    }
}

int move_snake(int direction, int snake[SNAKE_LENGTH][2]) {
    if (direction == UP) snake[0][0]--;
    else if (direction == RIGHT) snake[0][1]++;
    else if (direction == DOWN) snake[0][0]++;
    else if (direction == LEFT) snake[0][1]--;
    else return 1;
    for (int coord = 1; coord < SNAKE_LENGTH; ++coord) {
        int x = snake[coord][0];
        int y = snake[coord][1];
        if (x + y == 0) break;
        snake[coord][0] = snake[coord+1][0];
        snake[coord][1] = snake[coord+1][1];
    }
    return 0;
}

int main() {
    int board[WIDTH][HEIGHT];
    int snake[SNAKE_LENGTH][2] = {
            {5, 5},
            { 5, 6},
            { 5, 7},
            { 5, 8}
    };
    set_board(board);
    apply_snake_coords(snake, board);
    draw_board(board);
    move_snake(UP, snake);
    apply_snake_coords(snake, board);
    draw_board(board);
    move_snake(UP, snake);
    apply_snake_coords(snake, board);
    draw_board(board);
    move_snake(RIGHT, snake);
    apply_snake_coords(snake, board);
    draw_board(board);
    return 0;
}