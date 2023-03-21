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

/**
 * performs print operation to print the bytes in the board matrix
 */
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

/**
 * sets every board character into the board matrix
 */
void reset_board(int board[WIDTH][HEIGHT]) {
    for (int w = 0; w < WIDTH; ++w) {
        for (int h = 0; h < HEIGHT; ++h) {
            if (w == 0) board[w][h] = '_';
            else if (w == WIDTH-1) board[w][h] = '-';
            else if (h == 0 || h == HEIGHT-1) board[w][h] = '|';
            else board[w][h] = '~';
        }
    }
}

void apply_snake_to_board(int snake[SNAKE_LENGTH][2], int board[WIDTH][HEIGHT]) {
    for (int r = 0; r < SNAKE_LENGTH; ++r) {
        int x = snake[r][0];
        int y = snake[r][1];
        if (x + y == 0) return;
        board[x][y] = 'X';
    }
}

/**
 * performs left shift by 1 position in the snake matrix
 */
int move_snake(int direction, int snake[SNAKE_LENGTH][2], int snake_length) {
    for (int i=snake_length-1; i > 0; --i) {
        snake[i][0] = snake[i-1][0];
        snake[i][1] = snake[i-1][1];
    }
    if (direction == UP) snake[0][0]--;
    else if (direction == RIGHT) snake[0][1]++;
    else if (direction == DOWN) snake[0][0]++;
    else if (direction == LEFT) snake[0][1]--;
    else return 1;
    return 0;
}

int main() {
    int score = 0;
    int snake_length = 4;
    int board[WIDTH][HEIGHT];
    int snake[SNAKE_LENGTH][2] = {
            {5, 5},
            {5, 6},
            {5, 7},
            {5, 8}
    };
    reset_board(board);
    apply_snake_to_board(snake, board);
    draw_board(board);
    move_snake(UP, snake, snake_length);
    reset_board(board);
    apply_snake_to_board(snake, board);
    draw_board(board);
    move_snake(UP, snake, snake_length);
    reset_board(board);
    apply_snake_to_board(snake, board);
    draw_board(board);
    move_snake(RIGHT, snake, snake_length);
    reset_board(board);
    apply_snake_to_board(snake, board);
    draw_board(board);
    return 0;
}