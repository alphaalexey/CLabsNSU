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
    for (int i = 0; i < vCount * vCount; i++) {
        graph->adjacencyMatrix[i] = -1;
    }
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
