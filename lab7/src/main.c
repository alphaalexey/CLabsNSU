#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "stack.h"

int is_good_n(int n) { return n >= 0 && n <= 2000; }

int is_good_m(int n, int m) { return m >= 0 && m <= (n * (n - 1)) / 2; }

int is_good_vertex(int N, int number) { return number >= 1 && number <= N; }

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) {
        puts("bad number of lines");
        return EXIT_SUCCESS;
    }
    if (!is_good_n(N)) {
        puts("bad number of vertices");
        return EXIT_SUCCESS;
    }

    int M;
    if (scanf("%d", &M) != 1) {
        puts("bad number of lines");
        return EXIT_SUCCESS;
    }
    if (!is_good_m(N, M)) {
        puts("bad number of edges");
        return EXIT_SUCCESS;
    }

    Graph *graph = InitGraph(N);

    for (int i = 0; i < M; i++) {
        int from, to;
        if (scanf("%d %d", &from, &to) != 2) {
            FreeGraph(graph);
            puts("bad number of lines");
            return EXIT_SUCCESS;
        }
        if (!is_good_vertex(N, from) || !is_good_vertex(N, to)) {
            FreeGraph(graph);
            puts("bad vertex");
            return EXIT_SUCCESS;
        }

        AddGraphEdge(graph, from - 1, to - 1);
    }

    Stack *stack;
    if (GetGraphTopologicalSort(graph, &stack) < 0) {
        FreeGraph(graph);
        puts("impossible to sort");
        return EXIT_SUCCESS;
    }

    int value;
    while (!PopStack(stack, &value)) {
        printf("%d ", value + 1);
    }
    puts("");

    FreeStack(stack);
    FreeGraph(graph);
    return EXIT_SUCCESS;
}
