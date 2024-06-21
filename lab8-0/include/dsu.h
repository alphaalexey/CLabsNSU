#pragma once

typedef struct DSU DSU;

DSU *InitDSU(int vCount);
void FreeDSU(DSU *dsu);
int FindDSU(DSU *dsu, int v);
void UnionDSU(DSU *dsu, int v1, int v2);
