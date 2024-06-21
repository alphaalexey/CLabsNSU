#pragma once

typedef struct Graph Graph;

Graph *InitGraph(int vCount);
void FreeGraph(Graph *graph);
void AddGraphEdge(Graph *graph, int from, int to, int length);
int GetGraphEdge(Graph *graph, int from, int to);
int *CreateMST(Graph *graph);
