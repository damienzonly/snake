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
	width = 60;
	height = 30;
	max_snake_length = ((width-3) * (height-2)) - 1; // width minus borders minus newfile char, height minus borders, minus 1 for 0-based indexes
}

void draw(int snake_coordinates[][2], int rows) {
    printf("┌");
    for (int j = 0; j < width; ++j) {
        printf("─");
    }
    printf("┐\n");

    for (int j = 0; j < height; j++) {
        printf("│");
        for (int i = 0; i < width; i++)
            printf("·");
        printf("│\n");
    }
    printf("└");
    for (int i = 0; i < width; i++)
        printf("─");
    printf("┘\n");

//	for (int h=0; h<height; ++h)
//		for (int w=0; w<width; ++w)
//			putc(buf[h][w], stdout);
}

void main() {
	int snake_buf[27][2];//= {{0}}; // matrix of snake coordinates
//	printf("ciao %d\n", snake_buf[1][1]);
    snake_buf[0][0] = 4;
    snake_buf[0][1] = 5;
    snake_buf[1][0] = 5;
    snake_buf[1][1] = 5;
    init();
	while (1) {
		draw(snake_buf, 2);
		msleep(500);
        break;
		system("clear");
	}
}