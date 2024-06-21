#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

typedef struct Node {
    char *key;
    int is_word;
    struct Node *brother;
    struct Node *child;
} Node;

static Node *init_node(const char *key, int is_word) {
    Node *node = calloc(1, sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->key = malloc(strlen(key) + 1);
    if (node->key == NULL) {
        free(node);
        return NULL;
    }
    strcpy(node->key, key);
    node->is_word = is_word;
    return node;
}

static void free_node(Node *node) {
    if (node == NULL) {
        return;
    }
    free(node->key);
    if (node->brother != NULL) {
        free_node(node->brother);
    }
    if (node->child != NULL) {
        free_node(node->child);
    }
    free(node);
}

static void add_node_child(Node *node, Node *child) {
    if (node->child == NULL) {
        node->child = child;
    } else {
        Node *brother = node->child;
        while (brother->brother != NULL) {
            brother = brother->brother;
        }
        brother->brother = child;
    }
}

static Node *find_node_child(Node *node, char key) {
    Node *child = node->child;
    while (child != NULL) {
        if (child->key[0] == key) {
            return child;
        }
        child = child->brother;
    }
    return NULL;
}

static void split_node(Node *node, int position) {
    Node *new_node = init_node(node->key + position, node->is_word);
    if (new_node == NULL) {
        return;
    }
    node->key[position] = '\0';
    new_node->is_word = node->is_word;
    node->is_word = 0;
    new_node->child = node->child;
    node->child = new_node;
}

struct Trie {
    Node *root;
    int count;
};

Trie *InitTrie(void) {
    Trie *trie = calloc(1, sizeof(*trie));
    if (trie == NULL) {
        return NULL;
    }
    return trie;
}

void FreeTrie(Trie *trie) {
    free_node(trie->root);
    free(trie);
}

int GetTrieCount(const Trie *trie) { return trie->count; }

static void trie_insert_node(Trie *trie, const char *key) {
    Node *node = trie->root;
    int posisition, offset = 0;
    for (posisition = 0; key[posisition] != '\0'; posisition++) {
        if (node->key[posisition - offset] == '\0') {
            Node *next_node = find_node_child(node, key[posisition]);
            if (next_node == NULL) {
                add_node_child(node, init_node(key + posisition, 1));
                trie->count++;
                return;
            }
            node = next_node;
            offset = posisition;
        }
        if (node->key[posisition - offset] != key[posisition]) {
            break;
        }
    }

    if (node->key[posisition - offset] == '\0') {
        node->is_word = 1;
    } else {
        if (key[posisition] != '\0') {
            split_node(node, posisition - offset);
            add_node_child(node, init_node(key + posisition, 1));
            trie->count++;
        } else {
            split_node(node, posisition - offset);
            node->is_word = 1;
        }
        trie->count++;
    }
}

void TrieInsert(Trie *trie, const char *key) {
    if (trie->root == NULL) {
        trie->root = init_node(key, 1);
        trie->count++;
    } else {
        trie_insert_node(trie, key);
    }
}

static int trie_traversal_print(Node *node, char *word, int position) {
    if (node == NULL) {
        return 0;
    }

    int ret = 0;

    strcpy(word + position, node->key);
    if (node->is_word) {
        printf("%s ", word);
        ret = 1;
    }

    int length = strlen(node->key);
    for (Node *child = node->child; child != NULL; child = child->brother) {
        ret |= trie_traversal_print(child, word, position + length);
    }
    return ret;
}

void TrieSearchAndPrint(Trie *trie, const char *prefix) {
    Node *node = trie->root;
    int posisition, offset = 0;
    for (posisition = 0; prefix[posisition] != '\0'; posisition++) {
        if (node->key[posisition - offset] == '\0') {
            node = find_node_child(node, prefix[posisition]);
            if (node == NULL) {
                goto not_found;
            }
            offset = posisition;
        }
        if (node->key[posisition - offset] != prefix[posisition]) {
            goto not_found;
        }
    }

    char *word = malloc(10001 * sizeof(word[0]));
    if (word == NULL) {
        return;
    }
    strncpy(word, prefix, offset);

    int found = trie_traversal_print(node, word, offset);
    free(word);
    if (found) {
        puts("");
        return;
    }

not_found:
    puts("None");
}
