#include <stdio.h>
#include <stdlib.h>

#include "trie.h"

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) {
        return EXIT_SUCCESS;
    }
    if (N == 0) {
        puts("None\n0");
        return EXIT_SUCCESS;
    }

    Trie *trie = InitTrie();

    char key[10001] = {0};
    for (int i = 0; i < N; i++) {
        if (scanf("%10000s", key) != 1) {
            break;
        };

        TrieInsert(trie, key);
    }

    char prefix[10001] = {0};
    if (scanf("%10000s", prefix) != 1) {
        FreeTrie(trie);
        return EXIT_SUCCESS;
    };

    TrieSearchAndPrint(trie, prefix);
    printf("%d\n", GetTrieCount(trie));

    FreeTrie(trie);

    return EXIT_SUCCESS;
}
