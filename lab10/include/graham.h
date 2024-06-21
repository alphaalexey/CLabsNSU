#pragma once

#include "stack.h"

typedef struct Point {
    int x, y;
} Point;

Stack *graham(int N, Point *points);
