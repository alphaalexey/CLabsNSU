#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

int main(void) {
    int factor, N;
    if (scanf("%i %i", &factor, &N) != 2) {
        return EXIT_SUCCESS;
    }

    BTree *btree = InitBTree(factor);

    for (int i = 0; i < N; i++) {
        int input;
        if (scanf("%d", &input) != 1) {
            break;
        };

        BTreeInsert(btree, input);
    }

    printf("%d\n", BTreeHeight(btree));

    FreeBTree(btree);

    return EXIT_SUCCESS;
}
