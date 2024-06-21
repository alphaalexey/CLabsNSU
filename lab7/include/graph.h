#pragma once

#include "stack.h"

typedef struct Graph Graph;

Graph *InitGraph(int vCount);
void FreeGraph(Graph *graph);
int AddGraphEdge(Graph *graph, int from, int to);
int GetGraphEdge(Graph *graph, int from, int to);
int IsAcyclicGraph(Graph *graph);
int GetGraphTopologicalSort(Graph *graph, Stack **stack);
