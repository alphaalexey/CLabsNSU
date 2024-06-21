#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bit_set.h"
#include "graph.h"

typedef enum { SUCCESS, NO_PATH, OVERFLOW } Result;

static int is_good_n(int N) { return N >= 0 && N <= 5000; }

static int is_good_m(int N, int M) { return M >= 0 && M <= (N * (N - 1)) / 2; }

static int is_good_vertex(int N, int number) {
    return number >= 1 && number <= N;
}

static int is_good_length(long length) {
    return length >= 0 && length <= INT_MAX;
}

static int get_min_edge(const long distance[], Bitset *usedVertices, int N) {
    long min = LONG_MAX;
    int min_index = 0;
    for (int i = 0; i < N; i++) {
        if (!GetBitsetBit(usedVertices, i) && distance[i] < min) {
            min = distance[i];
            min_index = i;
        }
    }
    return min_index;
}

Result dijkstra(int N, int S, int F, Graph *graph, long distance[],
                int parent[]) {
    Bitset *usedVertices = InitBitset(N);
    for (int i = 0; i < N; i++) {
        distance[i] = LONG_MAX;
    }
    distance[S] = 0;

    for (int i = 0; i < N; i++) {
        int u = get_min_edge(distance, usedVertices, N);
        SetBitsetBit(usedVertices, u);
        for (int j = 0; j < N; j++) {
            long length = GetGraphEdge(graph, u, j);
            if (length != -1 && !GetBitsetBit(usedVertices, j) &&
                distance[u] + length < distance[j]) {
                distance[j] = distance[u] + length;
                parent[j] = u;
            }
        }
    }

    if (distance[F] == LONG_MAX) {
        FreeBitset(usedVertices);
        return NO_PATH;
    }

    if (distance[F] > INT_MAX) {
        for (int i = 0; i < N; i++) {
            long length = GetGraphEdge(graph, F, i);
            if (length != -1 && distance[i] + length > INT_MAX &&
                i != parent[F]) {
                FreeBitset(usedVertices);
                return OVERFLOW;
            }
        }
    }

    FreeBitset(usedVertices);
    return 0;
}

int main(void) {
    int N;
    if (scanf("%i", &N) != 1) {
        goto error_number_of_lines;
    }
    if (!is_good_n(N)) {
        puts("bad number of vertices");
        return EXIT_SUCCESS;
    }

    int S, F;
    if (scanf("%i %i", &S, &F) != 2) {
        goto error_number_of_lines;
    }
    if (!is_good_vertex(N, S) || !is_good_vertex(N, F)) {
        puts("bad vertex");
        return EXIT_SUCCESS;
    }
    S--;
    F--;

    int M;
    if (scanf("%i", &M) != 1) {
        goto error_number_of_lines;
    }
    if (!is_good_m(N, M)) {
        puts("bad number of edges");
        return EXIT_SUCCESS;
    }

    Graph *graph = InitGraph(N);
    for (int i = 0; i < M; i++) {
        int from, to;
        long length;
        if (scanf("%i %i %li", &from, &to, &length) != 3) {
            FreeGraph(graph);
            goto error_number_of_lines;
        }
        if (!is_good_vertex(N, from) || !is_good_vertex(N, to)) {
            FreeGraph(graph);
            puts("bad vertices");
            return EXIT_SUCCESS;
        }
        if (!is_good_length(length)) {
            FreeGraph(graph);
            puts("bad length");
            return EXIT_SUCCESS;
        }
        AddGraphEdge(graph, from - 1, to - 1, length);
    }

    int *parent = malloc(N * sizeof(parent[0]));
    long *distance = malloc(N * sizeof(distance[0]));
    long result = dijkstra(N, S, F, graph, distance, parent);
    for (int i = 0; i < N; i++) {
        if (distance[i] == LONG_MAX) {
            printf("oo ");
        } else if (distance[i] > INT_MAX) {
            printf("INT_MAX+ ");
        } else {
            printf("%li ", distance[i]);
        }
    }
    printf("\n");
    switch (result) {
    case NO_PATH:
        puts("no path");
        break;
    case OVERFLOW:
        puts("overflow");
        break;
    default:
        for (int i = F; i != S; i = parent[i]) {
            printf("%i ", i + 1);
        }
        printf("%i\n", S + 1);
    }

    free(distance);
    free(parent);
    FreeGraph(graph);
    return EXIT_SUCCESS;

error_number_of_lines:
    puts("bad number of lines");
    return EXIT_SUCCESS;
}
