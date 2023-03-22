#define _UP 0
#define _RIGHT 1
#define _DOWN 2
#define _LEFT 3
#define _COLS 10
#define _ROWS 5
#define _WIDTH _ROWS+2 // rows
#define _HEIGHT _COLS+2 // columns
#define _SNAKE_LENGTH (_WIDTH-2)*(_HEIGHT-2)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <ncurses.h>

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
void draw_board(int board[_WIDTH][_HEIGHT]) {
    msleep(100);
    system("clear");
    for (int i = 0; i < _WIDTH; ++i) {
        for (int j = 0; j < _HEIGHT; ++j) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

/**
 * sets every board character into the board matrix
 */
void reset_board(int board[_WIDTH][_HEIGHT]) {
    for (int w = 0; w < _WIDTH; ++w) {
        for (int h = 0; h < _HEIGHT; ++h) {
            if (w == 0) board[w][h] = '_';
            else if (w == _WIDTH - 1) board[w][h] = '-';
            else if (h == 0 || h == _HEIGHT - 1) board[w][h] = '|';
            else board[w][h] = '~';
        }
    }
}

void apply_snake_to_board(int snake[_SNAKE_LENGTH][2], int board[_WIDTH][_HEIGHT]) {
    for (int r = 0; r < _SNAKE_LENGTH; ++r) {
        int x = snake[r][0];
        int y = snake[r][1];
        if (x + y == 0) return;
        board[x][y] = 'X';
    }
}

/**
 * performs left shift by 1 position in the snake matrix
 */
int move_snake(int direction, int snake[_SNAKE_LENGTH][2], int snake_length) {
    for (int i=snake_length-1; i > 0; --i) {
        snake[i][0] = snake[i-1][0];
        snake[i][1] = snake[i-1][1];
    }
    if (direction == _UP) snake[0][0] = (snake[0][0] - 1) % (_ROWS + 1);
    else if (direction == _RIGHT) snake[0][1] = (snake[0][1] + 1) % (_COLS + 1);
    else if (direction == _LEFT) snake[0][0] = (snake[0][0] + 1) % (_ROWS + 1);
    else if (direction == _DOWN) snake[0][1] = (snake[0][1] - 1) % (_COLS + 1);
    else return 1;
    return 0;
}

int main() {
    int score = 0;
    int snake_length = 4;
    int board[_WIDTH][_HEIGHT];
    int snake[_SNAKE_LENGTH][2] = {
            {5, 5},
            {5, 6},
            {5, 7},
            {5, 8}
    };
    initscr();                  // initialize ncurses
//    cbreak();                   // disable line buffering
    noecho();                   // disable echoing of typed characters
    nodelay(stdscr, TRUE);      // make input non-blocking
    timeout(0);
//    setvbuf(stdout, NULL, _IONBF, 0);
    while (1) {
        reset_board(board);
        apply_snake_to_board(snake, board);
        draw_board(board);
        char ch = getch();
        if (ch != ERR) {
            printf("char %c", ch);
            if (ch == 'w') {
                move_snake(_UP, snake, snake_length);
            } else if (ch == 'a') {
                move_snake(_DOWN, snake, snake_length);
            } else if (ch == 's') {
                move_snake(_LEFT, snake, snake_length);
            } else if (ch == 'd') {
                move_snake(_RIGHT, snake, snake_length);
            }
        }
        endwin();
    }
    return 0;
}