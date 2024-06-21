#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct _Node {
    int data;
    int height;
    struct _Node *left;
    struct _Node *right;
} Node;

int getHeight(Node *rootNode) { return rootNode ? rootNode->height : 0; }

int calculateBalanceFactor(Node *rootNode) {
    return getHeight(rootNode->right) - getHeight(rootNode->left);
}

void calculateHeights(Node *rootNode) {
    int height_left = getHeight(rootNode->left);
    int height_right = getHeight(rootNode->right);
    rootNode->height = MAX(height_left, height_right) + 1;
}

Node *rotateRight(Node *rootNode) {
    Node *newRoot = rootNode->left;
    rootNode->left = newRoot->right;
    newRoot->right = rootNode;

    calculateHeights(rootNode);
    calculateHeights(newRoot);

    return newRoot;
}

Node *rotateLeft(Node *rootNode) {
    Node *newRoot = rootNode->right;
    rootNode->right = newRoot->left;
    newRoot->left = rootNode;

    calculateHeights(rootNode);
    calculateHeights(newRoot);

    return newRoot;
}

Node *rebalance(Node *rootNode) {
    calculateHeights(rootNode);

    if (calculateBalanceFactor(rootNode) == 2) {
        if (calculateBalanceFactor(rootNode->right) < 0) {
            rootNode->right = rotateRight(rootNode->right);
        }
        rootNode = rotateLeft(rootNode);
    }
    
    if (calculateBalanceFactor(rootNode) == -2) {
        if (calculateBalanceFactor(rootNode->left) > 0) {
            rootNode->left = rotateLeft(rootNode->left);
        }
        rootNode = rotateRight(rootNode);
    }

    return rootNode;
}

Node *insert(Node *rootNode, Node *newNode) {
    if (rootNode == NULL) {
        return newNode;
    }

    if (newNode->data < rootNode->data) {
        rootNode->left = insert(rootNode->left, newNode);
    } else {
        rootNode->right = insert(rootNode->right, newNode);
    }

    return rebalance(rootNode);
}

int main(void) {
    size_t N;
    if (scanf("%zu", &N) != 1) {
        return EXIT_SUCCESS;
    }

    Node *array = malloc(N * sizeof(Node));
    if (array == NULL) {
        return EXIT_FAILURE;
    }

    Node *rootNode = NULL;
    for (size_t i = 0; i < N; i++) {
        int input;
        if (scanf("%d", &input) != 1) {
            break;
        }

        Node *newNode = array + i;
        newNode->data = input;
        newNode->height = 1;
        newNode->left = NULL;
        newNode->right = NULL;

        rootNode = insert(rootNode, newNode);
    }
    printf("%d\n", getHeight(rootNode));

    free(array);

    return EXIT_SUCCESS;
}
