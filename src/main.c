#define MOVE_UP 0
#define MOVE_RIGHT 1
#define MOVE_BOTTOM 2
#define MOVE_LEFT 3

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

char snake_char = 'x';
char star = '*';
char space = ' ';
char price_char = '$';


void init() {
	width = 30;
	height = width-1;
	max_snake_length = ((width-3) * (height-2)) - 1;
}

void draw(int snake_coordinates[][2], int rows) {
	char buf[height][width+1]; // +1 for newline char
	for (int h=0; h<height; ++h) {
		for (int w=0; w<width; ++w) {
			for (int i=0; i<rows; ++i) {
				int x, y;
				x = snake_coordinates[i][0];
				y = snake_coordinates[i][1];
				// printf("x, y | %d %d\n", x, y);
				if (h == 0 || h == height-1 || w == 0 || w == width-2) buf[h][w] = star;
				else if (x == w && y == h) {
					printf("hit %d %d\n", x, y);
					buf[h][w] = snake_char;

				}
				else buf[h][w] = space;
			}
		}
		buf[h][width-1] = '\n';
	}

	for (int h=0; h<height; ++h)
		for (int w=0; w<width; ++w)
			putc(buf[h][w], stdout);
}

void main() {
	int snake_buf[2][2] = {0}; // matrix of snake coordinates
	snake_buf[0][0] = 4;
	snake_buf[0][1] = 5;
	snake_buf[1][0] = 5;
	snake_buf[1][1] = 5;
	
	printf("ciao %d\n", snake_buf[1][1]);
	init();
	while (1) {
		draw(snake_buf, 2);
		msleep(100);
		break;
		system("clear");
	}
}