#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsu.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct Edge {
    int length;
    int from, to;
} Edge;

static int is_good_n(int n) { return n >= 0 && n <= 5000; }

static int is_good_m(int n, int m) { return m >= 0 && m <= (n * (n - 1)) / 2; }

static int is_good_vertex(int N, int number) {
    return number >= 1 && number <= N;
}

static int is_good_length(long length) {
    return length >= 0 && length <= INT_MAX;
}

static int edge_sort(const void *a, const void *b) {
    const Edge *edge_a = (Edge *)a;
    const Edge *edge_b = (Edge *)b;
    return edge_a->length > edge_b->length;
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

    if (N == 0) {
        puts("no spanning tree");
        return EXIT_SUCCESS;
    }

    if (N == 1 && M == 0) {
        return EXIT_SUCCESS;
    }

    Edge *edges = calloc(M, sizeof(edges[0]));
    for (int i = 0; i < M; i++) {
        int a, b;
        long length;
        if (scanf("%d %d %ld", &a, &b, &length) != 3) {
            free(edges);
            goto error_number_of_lines;
        }
        if (!is_good_vertex(N, a) || !is_good_vertex(N, b)) {
            free(edges);
            puts("bad vertex");
            return EXIT_SUCCESS;
        }
        if (!is_good_length(length)) {
            free(edges);
            puts("bad length");
            return EXIT_SUCCESS;
        }
        edges[i].from = MIN(a, b) - 1;
        edges[i].to = MAX(a, b) - 1;
        edges[i].length = length;
    }

    qsort(edges, M, sizeof(edges[0]), edge_sort);

    Edge *tree = calloc(N - 1, sizeof(tree[0]));
    DSU *dsu = InitDSU(N);

    int edges_count = 0;
    int vertices_count = 0;
    while (edges_count < M && vertices_count < N) {
        int set_a = FindDSU(dsu, edges[edges_count].from);
        int set_b = FindDSU(dsu, edges[edges_count].to);
        if (set_a != set_b) {
            tree[vertices_count++] = edges[edges_count];
            UnionDSU(dsu, set_a, set_b);
        }
        edges_count++;
    }

    if (vertices_count != N - 1) {
        puts("no spanning tree");
    } else {
        for (int i = 0; i < N - 1; i++) {
            printf("%d %d\n", tree[i].from + 1, tree[i].to + 1);
        }
    }

    FreeDSU(dsu);
    free(tree);
    free(edges);

    return EXIT_SUCCESS;

error_number_of_lines:
    puts("bad number of lines");
    return EXIT_SUCCESS;
}
