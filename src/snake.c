#include <stdio.h>
#include "data_structures.h"

void draw(int boardW, int boardY, Snake* snake) {
    int i = 0, j = 0;
    while (i++ < boardW + 1)
        printf("#");
    printf("\n");
    while (j++ < boardY) {
        i = 0;
        printf("#");
        while (i++ < boardW-1)
            printf(" ");
        printf("#\n");
    }
    i = 0;
    while (i++ < boardW + 1)
        printf("#");
    printf("\n");
}