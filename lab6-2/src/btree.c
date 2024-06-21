#include <stdlib.h>
#include <string.h>

#include "btree.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct Node {
    int keyCount;
    int *keys;
    struct Node **children;
} Node;

struct BTree {
    int factor;
    Node *root;
};

static Node *InitNode(int factor, int isLeaf) {
    Node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->keyCount = 0;
    node->keys = malloc((2 * factor - 1) * sizeof(node->keys[0]));
    if (node->keys == NULL) {
        free(node);
        return NULL;
    }
    if (!isLeaf) {
        node->children = calloc(2 * factor, sizeof(node->children[0]));
        if (node->children == NULL) {
            free(node->keys);
            free(node);
            return NULL;
        }
    } else {
        node->children = NULL;
    }
    return node;
}

static void FreeNode(Node *node) {
    if (node == NULL) {
        return;
    }
    free(node->keys);
    if (node->children != NULL) {
        for (int i = 0; i <= node->keyCount; i++) {
            FreeNode(node->children[i]);
        }
        free(node->children);
    }
    free(node);
}

static int IsLeafNode(Node *node) { return node->children == NULL; }

static int GetNodeHeight(Node *node) {
    if (node == NULL) {
        return 0;
    }

    if (IsLeafNode(node)) {
        return node->keyCount > 0;
    }

    int height = 0;
    for (int i = 0; i < node->keyCount; i++) {
        height = MAX(height, GetNodeHeight(node->children[i]) + 1);
    }
    return height;
}

static int GetNodeIndex(Node *node, int key) {
    int left = 0;
    int right = node->keyCount - 1;
    while (left <= right) {
        int middle = (left + right) / 2;
        if (key < node->keys[middle]) {
            right = middle - 1;
        } else {
            left = middle + 1;
        }
    }
    return right + 1;
}

static void NodeSplit(Node *node, int index, int factor) {
    Node *oldNode = node->children[index];
    Node *newNode = InitNode(factor, IsLeafNode(oldNode));
    newNode->keyCount = factor - 1;
    oldNode->keyCount = factor - 1;

    memmove(&(newNode->keys[0]), &(oldNode->keys[factor]),
            newNode->keyCount * sizeof(oldNode->keys[0]));
    if (!IsLeafNode(oldNode)) {
        memmove(&(newNode->children[0]), &(oldNode->children[factor]),
                (newNode->keyCount + 1) * sizeof(oldNode->children[0]));
    }

    memmove(&(node->keys[index + 1]), &(node->keys[index]),
            (node->keyCount - index) * sizeof(node->keys[0]));
    if (!IsLeafNode(node)) {
        memmove(&(node->children[index + 1]), &(node->children[index]),
                (node->keyCount - index + 1) * sizeof(node->children[0]));
    }

    node->keys[index] = oldNode->keys[factor - 1];
    node->keyCount++;
    node->children[index + 1] = newNode;
}

BTree *InitBTree(int factor) {
    BTree *btree = malloc(sizeof(*btree));
    if (btree == NULL) {
        return NULL;
    }
    btree->factor = factor;
    btree->root = InitNode(factor, 1);
    if (btree->root == NULL) {
        free(btree);
        return NULL;
    }
    return btree;
}

void FreeBTree(BTree *btree) {
    if (btree == NULL) {
        return;
    }
    FreeNode(btree->root);
    free(btree);
}

static int BTreeInsertAtNode(BTree *btree, Node *node, int key) {
    if (node == NULL || node->keyCount == 2 * btree->factor - 1) {
        return -1;
    }

    int index = GetNodeIndex(node, key);
    if (IsLeafNode(node)) {
        memmove(&(node->keys[index + 1]), &(node->keys[index]),
                (node->keyCount - index) * sizeof(node->keys[0]));
        node->keys[index] = key;
        node->keyCount++;
    } else {
        if (node->children[index]->keyCount == 2 * btree->factor - 1) {
            NodeSplit(node, index, btree->factor);
            index += key > node->keys[index];
        }
        if (BTreeInsertAtNode(btree, node->children[index], key)) {
            return -1;
        }
    }
    return 0;
}

int BTreeInsert(BTree *btree, int key) {
    if (btree == NULL) {
        return -1;
    }
    if (btree->root->keyCount == 2 * btree->factor - 1) {
        Node *newRoot = InitNode(btree->factor, 0);
        newRoot->children[0] = btree->root;
        NodeSplit(newRoot, 0, btree->factor);
        btree->root = newRoot;
    }
    return BTreeInsertAtNode(btree, btree->root, key);
}

int BTreeHeight(BTree *btree) {
    if (btree == NULL) {
        return -1;
    }
    return GetNodeHeight(btree->root);
}
