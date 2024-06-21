#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "bit_set.h"
#include "graph.h"

struct Graph {
    int vCount;
    int adjacencyMatrix[];
};

Graph *InitGraph(int vCount) {
    Graph *graph = malloc(sizeof(*graph) +
                          vCount * vCount * sizeof(graph->adjacencyMatrix[0]));
    graph->vCount = vCount;
    memset(graph->adjacencyMatrix, 0,
           vCount * vCount * sizeof(graph->adjacencyMatrix[0]));
    return graph;
}

void FreeGraph(Graph *graph) { free(graph); }

void AddGraphEdge(Graph *graph, int from, int to, int length) {
    graph->adjacencyMatrix[from * graph->vCount + to] = length;
    graph->adjacencyMatrix[to * graph->vCount + from] = length;
}

int GetGraphEdge(Graph *graph, int from, int to) {
    return graph->adjacencyMatrix[from * graph->vCount + to];
}

static int get_min_edge(const long *lengths, Bitset *used, int N) {
    long min = LONG_MAX;
    int min_index = -1;
    for (int i = 0; i < N; i++) {
        if ((!GetBitsetBit(used, i)) && (lengths[i] < min)) {
            min = lengths[i];
            min_index = i;
        }
    }
    return min_index;
}

static int is_good_vertex(int N, int number) {
    return number >= 1 && number <= N;
}

int *CreateMST(Graph *graph) {
    int *mst = malloc(graph->vCount * sizeof(mst[0]));
    long *lengths = malloc(graph->vCount * sizeof(lengths[0]));
    Bitset *used = InitBitset(graph->vCount);
    int used_count = 0;

    for (int i = 0; i < graph->vCount; i++) {
        lengths[i] = LONG_MAX;
    }
    mst[0] = -1;
    lengths[0] = 0;

    int index = 0;
    for (int i = 0; i < graph->vCount; i++) {
        int min_vertex = get_min_edge(lengths, used, graph->vCount);
        if (is_good_vertex(graph->vCount, min_vertex + 1)) {
            SetBitsetBit(used, min_vertex);
            used_count++;
        }
        for (int j = 0; j < graph->vCount; j++) {
            if (min_vertex != -1) {
                index = GetGraphEdge(graph, min_vertex, j);
            }
            if (index != 0 && !GetBitsetBit(used, j) && index < lengths[j]) {
                mst[j] = min_vertex;
                lengths[j] = index;
            }
        }
    }

    FreeBitset(used);
    free(lengths);

    if (used_count != graph->vCount) {
        free(mst);
        return NULL;
    }

    return mst;
}
