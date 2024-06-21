#include <stdio.h>
#include <stdlib.h>

#include "graham.h"

static int is_good_n(int n) { return n >= 0 && n <= 100000; }

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) {
        goto error_number_of_lines;
    }
    if (!is_good_n(N)) {
        puts("bad number of points");
        return EXIT_SUCCESS;
    }
    if (N == 0) {
        return EXIT_SUCCESS;
    }

    Point *points = malloc(N * sizeof(Point));
    for (int i = 0; i < N; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2) {
            free(points);
            goto error_number_of_lines;
        }

        points[i].x = x;
        points[i].y = y;
    }

    Stack *hull = graham(N, points);
    if (hull == NULL) {
        free(points);
        return EXIT_SUCCESS;
    }
    int index;
    while (!PopStack(hull, &index)) {
        printf("%d %d\n", points[index].x, points[index].y);
    }

    FreeStack(hull);
    free(points);
    return EXIT_SUCCESS;

error_number_of_lines:
    puts("bad number of lines");
    return EXIT_SUCCESS;
}
