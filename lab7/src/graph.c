#include <stdint.h>
#include <stdlib.h>

#include "bit_set.h"
#include "graph.h"

struct Graph {
    Bitset *adjacencyMatrix;
    int vCount;
};

Graph *InitGraph(int vCount) {
    Graph *graph = malloc(sizeof(*graph));
    graph->adjacencyMatrix = InitBitset(vCount * vCount);
    graph->vCount = vCount;
    return graph;
}

void FreeGraph(Graph *graph) {
    FreeBitset(graph->adjacencyMatrix);
    free(graph);
}

int AddGraphEdge(Graph *graph, int from, int to) {
    return SetBitsetBit(graph->adjacencyMatrix, from * graph->vCount + to);
}

int GetGraphEdge(Graph *graph, int from, int to) {
    return GetBitsetBit(graph->adjacencyMatrix, from * graph->vCount + to);
}

int IsAcyclicGraphDFS(Graph *graph, uint8_t *color, int vertex) {
    color[vertex] = 1;
    for (int i = 0; i < graph->vCount; i++) {
        int edge = GetGraphEdge(graph, vertex, i);
        if (edge == -1) {
            return 0;
        } else if (edge == 1) {
            if (color[i] == 1 ||
                (color[i] == 0 && !IsAcyclicGraphDFS(graph, color, i))) {
                return 0;
            }
        }
    }
    color[vertex] = 2;
    return 1;
}

int IsAcyclicGraph(Graph *graph) {
    int result = 1;
    uint8_t *color = calloc(graph->vCount, sizeof(color[0]));

    for (int i = 0; i < graph->vCount; i++) {
        if (color[i] == 0 && !IsAcyclicGraphDFS(graph, color, i)) {
            result = 0;
            break;
        }
    }

    free(color);
    return result;
}

void GetGraphTopologicalSortDFS(Graph *graph, Stack *stack, int *visited,
                                int vertex) {
    visited[vertex] = 1;
    for (int i = 0; i < graph->vCount; i++) {
        if (GetGraphEdge(graph, vertex, i)) {
            if (!visited[i]) {
                GetGraphTopologicalSortDFS(graph, stack, visited, i);
            }
        }
    }
    PushStack(stack, vertex);
}

int GetGraphTopologicalSort(Graph *graph, Stack **stack) {
    if (!IsAcyclicGraph(graph)) {
        return -1;
    }

    *stack = InitStack();
    int *visited = calloc(graph->vCount, sizeof(visited[0]));
    for (int i = 0; i < graph->vCount; i++) {
        if (!visited[i]) {
            GetGraphTopologicalSortDFS(graph, *stack, visited, i);
        }
    }
    free(visited);
    return 0;
}
