#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bit_set.h"
#include "graph.h"

static int is_good_n(int n) { return n >= 0 && n <= 5000; }

static int is_good_m(int n, int m) { return m >= 0 && m <= (n * (n - 1)) / 2; }

static int is_good_vertex(int N, int number) {
    return number >= 1 && number <= N;
}

static int is_good_length(long length) {
    return length >= 0 && length <= INT_MAX;
}

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) {
        goto error_number_of_lines;
    }
    if (!is_good_n(N)) {
        puts("bad number of vertices");
        return EXIT_SUCCESS;
    }

    int M;
    if (scanf("%d", &M) != 1) {
        goto error_number_of_lines;
    }
    if (!is_good_m(N, M)) {
        puts("bad number of edges");
        return EXIT_SUCCESS;
    }

    if (N == 0 || N - 1 > M) {
        goto error_no_tree;
    }

    Graph *graph = InitGraph(N);
    for (int i = 0; i < M; i++) {
        int from, to;
        long length;
        if (scanf("%d %d %ld", &from, &to, &length) != 3) {
            FreeGraph(graph);
            goto error_number_of_lines;
        }
        if (!is_good_vertex(N, from) || !is_good_vertex(N, to)) {
            FreeGraph(graph);
            puts("bad vertex");
            return EXIT_SUCCESS;
        }
        if (!is_good_length(length)) {
            FreeGraph(graph);
            puts("bad length");
            return EXIT_SUCCESS;
        }
        if (from == to) {
            FreeGraph(graph);
            goto error_no_tree;
        }
        AddGraphEdge(graph, from - 1, to - 1, length);
    }

    int *mst = CreateMST(graph);
    if (mst == NULL) {
        free(mst);
        FreeGraph(graph);
        goto error_no_tree;
    }

    for (int i = 1; i < N; i++) {
        printf("%d %d\n", mst[i] + 1, i + 1);
    }

    free(mst);
    FreeGraph(graph);

    return EXIT_SUCCESS;

error_number_of_lines:
    puts("bad number of lines");
    return EXIT_SUCCESS;

error_no_tree:
    puts("no spanning tree");
    return EXIT_SUCCESS;
}
