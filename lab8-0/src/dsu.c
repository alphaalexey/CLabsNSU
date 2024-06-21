#include <stdlib.h>

#include "dsu.h"

struct DSU {
    int *parent;
    int *rank;
};

DSU *InitDSU(int vCount) {
    DSU *dsu = malloc(sizeof(*dsu));
    dsu->parent = calloc(vCount, sizeof(dsu->parent[0]));
    dsu->rank = calloc(vCount, sizeof(dsu->rank[0]));
    for (int i = 0; i < vCount; i++) {
        dsu->parent[i] = i;
        dsu->rank[i] = 0;
    }
    return dsu;
}

void FreeDSU(DSU *dsu) {
    free(dsu->parent);
    free(dsu->rank);
    free(dsu);
}

int FindDSU(DSU *dsu, int v) {
    if (dsu->parent[v] != v) {
        dsu->parent[v] = FindDSU(dsu, dsu->parent[v]);
    }
    return dsu->parent[v];
}

void UnionDSU(DSU *dsu, int v1, int v2) {
    int p1 = FindDSU(dsu, v1);
    int p2 = FindDSU(dsu, v2);
    if (p1 != p2) {
        if (dsu->rank[p1] < dsu->rank[p2]) {
            dsu->parent[p1] = p2;
        } else if (dsu->rank[p1] > dsu->rank[p2]) {
            dsu->parent[p2] = p1;
        } else {
            dsu->parent[p1] = p2;
            dsu->rank[p2]++;
        }
    }
}
