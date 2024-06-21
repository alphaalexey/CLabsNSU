#include <stdio.h>
#include <stdlib.h>

enum NodeColor { BLACK, RED };

typedef struct _Node {
    int data;
    enum NodeColor color;
    struct _Node *left;
    struct _Node *right;
} Node;

int getHeight(Node *node) {
    if (node == NULL) {
        return 1;
    }

    return getHeight(node->right) + (node->color == BLACK);
}

Node *rotateRight(Node *rootNode, Node *newNode, Node **relatives,
                  size_t index) {
    Node *temp = newNode->left;
    newNode->left = temp->right;
    temp->right = newNode;

    if (index == 0) {
        return temp;
    } else if (relatives[index - 1]->right == newNode) {
        relatives[index - 1]->right = temp;
    } else {
        relatives[index - 1]->left = temp;
    }

    return rootNode;
}

Node *rotateLeft(Node *rootNode, Node *newNode, Node **relatives,
                 size_t index) {
    Node *temp = newNode->right;
    newNode->right = temp->left;
    temp->left = newNode;

    if (index == 0) {
        return temp;
    } else if (relatives[index - 1]->left == newNode) {
        relatives[index - 1]->left = temp;
    } else {
        relatives[index - 1]->right = temp;
    }

    return rootNode;
}

Node *rebalance(Node *rootNode, Node *newNode, Node **relatives, size_t index) {
    Node *newRoot = rootNode;
    if (index > 0) {
        Node *parent = relatives[index - 1];
        if (parent->color == RED) {
            Node *grandParent = relatives[index - 2];
            if (grandParent->left == parent) {
                Node *uncle = grandParent->right;
                if (uncle && uncle->color == RED) {
                    uncle->color = BLACK;
                    parent->color = BLACK;
                    grandParent->color = RED;
                    if (index > 2) {
                        newRoot = rebalance(rootNode, grandParent, relatives,
                                            index - 2);
                    }
                } else {
                    if (parent->right == newNode) {
                        relatives[index] = parent;
                        rootNode =
                            rotateLeft(rootNode, parent, relatives, index - 1);
                        relatives[index - 1] = relatives[index - 2];
                        parent = relatives[index - 1];
                        grandParent = relatives[index - 2];
                    }
                    parent->color = BLACK;
                    grandParent->color = RED;
                    newRoot = rotateRight(rootNode, grandParent, relatives,
                                          index - 2);
                }
            } else {
                Node *uncle = grandParent->left;
                if (uncle && uncle->color == RED) {
                    uncle->color = BLACK;
                    parent->color = BLACK;
                    grandParent->color = RED;
                    if (index > 2) {
                        newRoot = rebalance(rootNode, grandParent, relatives,
                                            index - 2);
                    }
                } else {
                    if (parent->left == newNode) {
                        relatives[index] = parent;
                        rootNode =
                            rotateRight(rootNode, parent, relatives, index - 1);
                        relatives[index - 1] = relatives[index - 2];
                        parent = relatives[index - 1];
                        grandParent = relatives[index - 2];
                    }
                    parent->color = BLACK;
                    grandParent->color = RED;
                    newRoot =
                        rotateLeft(rootNode, grandParent, relatives, index - 2);
                }
            }
        }
    }

    newRoot->color = BLACK;

    return newRoot;
}

Node *insert(Node *rootNode, Node *newNode) {
    if (rootNode == NULL) {
        newNode->color = BLACK;
        return newNode;
    }

    Node *relatives[64];
    size_t length = 0;

    Node *currentNode = rootNode;
    Node *lastNode = currentNode;
    while (currentNode != NULL) {
        lastNode = currentNode;
        relatives[length++] = currentNode;
        if (newNode->data < currentNode->data) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }

    if (newNode->data < lastNode->data) {
        lastNode->left = newNode;
    } else {
        lastNode->right = newNode;
    }

    return rebalance(rootNode, newNode, relatives, length);
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
        };

        Node *newNode = array + i;
        newNode->data = input;
        newNode->color = RED;
        newNode->left = NULL;
        newNode->right = NULL;

        rootNode = insert(rootNode, newNode);
    }

    printf("%d\n", rootNode ? getHeight(rootNode) : 0);

    free(array);

    return EXIT_SUCCESS;
}
