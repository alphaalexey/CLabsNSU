#include <stdint.h>
#include <stdlib.h>

#include "graham.h"

static void swap(Point *a, Point *b) {
    Point tmp = *a;
    *a = *b;
    *b = tmp;
}

static Point *find_start(Point *points, int N) {
    Point *p0 = &points[0];
    for (int i = 1; i < N; i++) {
        if (points[i].x < p0->x ||
            (points[i].x == p0->x && points[i].y < p0->y)) {
            p0 = &points[i];
        }
    }
    return p0;
}

static int calc_orientation(Point *p, Point *q, Point *r) {
    int64_t a1 = (int64_t)q->y - (int64_t)p->y;
    int64_t a2 = (int64_t)r->x - (int64_t)q->x;
    int64_t a3 = (int64_t)q->x - (int64_t)p->x;
    int64_t a4 = (int64_t)r->y - (int64_t)q->y;
    double val = (double)a1 * (double)a2 - (double)a3 * (double)a4;
    if (val == 0) {
        return 0;
    }
    return (val > 0) ? 1 : 2;
}

static uint64_t distance(Point *p1, Point *p2) {
    uint64_t x = (int64_t)p1->x - (int64_t)p2->x;
    uint64_t y = (int64_t)p1->y - (int64_t)p2->y;
    return x * x + y * y;
}

static int compare_points(const void *a, const void *b, void *arg) {
    Point *p0 = (Point *)arg;
    Point *pa = (Point *)a;
    Point *pb = (Point *)b;

    int orientation = calc_orientation(p0, pa, pb);
    if (orientation == 0) {
        return distance(p0, pb) >= distance(p0, pa) ? -1 : 1;
    }
    return orientation == 2 ? -1 : 1;
}

/*
 *  Return codes:
 *      0 - OK, value stores int
 *      -1 - BAD, stack_len is 0
 *      -2 - BAD, stack_len is 1
 */
static int get_second_top(Stack *stack, int *value) {
    int top, secondTop;
    if (PopStack(stack, &top)) {
        return -1;
    }
    if (GetStackTop(stack, &secondTop)) {
        return -2;
    }
    PushStack(stack, top);
    *value = secondTop;
    return 0;
}

Stack *graham(int N, Point *points) {
    Point *p0 = find_start(points, N);
    swap(p0, &points[0]);
    p0 = &points[0];
    qsort_r(points + 1, N - 1, sizeof(points[0]), compare_points, &points[0]);

    int M = 1;
    for (int i = 1; i < N; i++) {
        while (i < N - 1 &&
               calc_orientation(p0, &points[i], &points[i + 1]) == 0) {
            i++;
        }
        points[M++] = points[i];
    }

    Stack *hull = InitStack();

    if (M < 3) {
        for (int i = 0; i < M; i++) {
            PushStack(hull, i);
        }
        return hull;
    }

    PushStack(hull, 0);
    PushStack(hull, 1);
    PushStack(hull, 2);
    for (int i = 3; i < M; i++) {
        int topIndex, nextIndex;
        for (;;) {
            int ret = get_second_top(hull, &nextIndex);
            if (ret == -2) {
                break;
            }
            if (GetStackTop(hull, &topIndex) || ret) {
                FreeStack(hull);
                return NULL;
            }
            if (calc_orientation(&points[nextIndex], &points[topIndex],
                                 &points[i]) == 2) {
                break;
            }
            PopStack(hull, NULL);
        }
        PushStack(hull, i);
    }

    return hull;
}
