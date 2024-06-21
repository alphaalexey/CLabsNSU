#include <stdio.h>
#include <stdlib.h>

#include "bit_set.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct Item {
    int weight;
    int cost;
} Item;

static void print_knapsack(Item *items, int N, int W) {
    // f[i,j]=max{f[i-1, j], f[i-1, j-weight[i]] + cost[i]}
    // optimal[i,j] is the optimal k for a given state
    int *optimal = malloc((N + 1) * (W + 1) * sizeof(optimal[0]));
    // iterate through the items [1; N]
    for (int i = 0; i < N + 1; i++) {
        // iterate through the capacity [1; W]
        for (int j = 0; j < W + 1; j++) {
            if (i == 0 || j == 0) {
                // empty knapsack
                optimal[i * (W + 1) + j] = 0;
            } else if (items[i - 1].weight <= j) {
                // we can take the item
                int a = items[i - 1].cost +
                        optimal[(i - 1) * (W + 1) + j - items[i - 1].weight];
                int b = optimal[(i - 1) * (W + 1) + j];
                optimal[i * (W + 1) + j] = MAX(a, b);
            } else {
                // we can't take the item. Keep the previous sum
                optimal[i * (W + 1) + j] = optimal[(i - 1) * (W + 1) + j];
            }
        }
    }

    int max = optimal[N * (W + 1) + W];
    int capacity = W;

    printf("%d\n", max);

    Bitset *bitset = InitBitset(N);
    for (int i = N; i > 0 && max > 0; i--) {
        if (max != optimal[(i - 1) * (W + 1) + capacity]) {
            SetBitsetBit(bitset, i - 1);

            max -= items[i - 1].cost;
            capacity -= items[i - 1].weight;
        }
    }
    for (int i = 0; i < N; i++) {
        if (GetBitsetBit(bitset, i)) {
            printf("%d %d\n", items[i].weight, items[i].cost);
        }
    }
    FreeBitset(bitset);

    free(optimal);
}

int main(void) {
    int N, W;
    if (scanf("%d %d", &N, &W) != 2) {
        puts("Cannot read input");
        return EXIT_FAILURE;
    }
    if (N == 0 || W == 0) {
        puts("0");
        return EXIT_SUCCESS;
    }

    Item *items = malloc(N * sizeof(Item));
    for (int i = 0; i < N; i++) {
        int weight, cost;
        if (scanf("%d %d", &weight, &cost) != 2) {
            free(items);
            puts("Cannot read input");
            return EXIT_FAILURE;
        }

        items[i].weight = weight;
        items[i].cost = cost;
    }

    print_knapsack(items, N, W);

    free(items);

    return EXIT_SUCCESS;
}
