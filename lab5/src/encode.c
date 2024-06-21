#include <stdint.h>
#include <stdlib.h>

#include "bit_set.h"
#include "bit_writer.h"
#include "encode.h"
#include "huffman.h"
#include "priority_queue.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define BUFFER_SIZE 512

EncodeNode *InitEncodeNode(uint8_t value, size_t frequency) {
    EncodeNode *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->frequency = frequency;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void FreeEncodeNode(EncodeNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->left != NULL) {
        FreeEncodeNode(node->left);
    }
    if (node->right != NULL) {
        FreeEncodeNode(node->right);
    }
    free(node);
}

static int calculate_frequencies(FILE *in, size_t frequencies[256]) {
    char *buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        return -1;
    }

    size_t length;
    while ((length = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, in)) > 0) {
        for (size_t i = 0; i < length; i++) {
            frequencies[(uint8_t)buffer[i]]++;
        }
    }
    free(buffer);

    return 0;
}

static EncodeNode *build_huffman_tree(PriorityQueue *queue, int *nodeCount) {
    *nodeCount = 0;
    for (;;) {
        EncodeNode *left = PriorityQueuePop(queue);
        if (left == NULL) {
            *nodeCount = 0;
            return NULL;
        }
        EncodeNode *right = PriorityQueuePop(queue);
        if (right == NULL) {
            return left;
        }
        EncodeNode *node =
            InitEncodeNode(0, left->frequency + right->frequency);
        node->height = MAX(left->height, right->height) + 1;
        node->left = left;
        node->right = right;
        (*nodeCount)++;
        PriorityQueueInsert(queue, node);
    }
}

static void build_huffman_codes(EncodeNode *root, Bitset *path,
                                Bitset *huffmanCodes[256], int depth) {
    if (root->left == NULL) {
        huffmanCodes[root->value] = CopyBitset(path, MAX(depth, 1));
    } else {
        ResetBitsetBit(path, depth);
        build_huffman_codes(root->left, path, huffmanCodes, depth + 1);

        SetBitsetBit(path, depth);
        build_huffman_codes(root->right, path, huffmanCodes, depth + 1);
    }
}

static void write_length(BitWriter *writer, int nodeCount,
                         const size_t frequencies[256],
                         Bitset *huffmanCodes[256]) {
    size_t length = 3 + nodeCount;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] != 0) {
            length += frequencies[i] * GetBitsetLength(huffmanCodes[i]);
            length += 9;
        }
    }
    WriteBits(writer, length % 8, 3);
}

static void write_tree(BitWriter *writer, EncodeNode *root, int depth) {
    if (root->left == NULL) {
        WriteBit(writer, 1);
        WriteBits(writer, root->value, 8);
    } else {
        WriteBit(writer, 0);
        write_tree(writer, root->left, depth + 1);
        write_tree(writer, root->right, depth + 1);
    }
}

static void write_data(BitWriter *writer, Bitset *huffmanCodes[256], FILE *in) {
    char *buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        return;
    }

    size_t length;
    while ((length = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, in)) > 0) {
        for (size_t i = 0; i < length; i++) {
            WriteBitSet(writer, huffmanCodes[(uint8_t)buffer[i]]);
        }
    }
    free(buffer);
}

int Encode(FILE *in, FILE *out) {
    long pos = ftell(in);

    size_t frequencies[256] = {0};
    if (calculate_frequencies(in, frequencies)) {
        return -1;
    }
    PriorityQueue *queue = InitPriorityQueue(frequencies);
    if (queue == NULL) {
        return -2;
    }

    int nodeCount; // number of non-leaf nodes in the tree
    EncodeNode *root = build_huffman_tree(queue, &nodeCount);
    FreePriorityQueue(queue);
    if (root == NULL) {
        return 0;
    }

    Bitset **huffmanCodes = malloc(256 * sizeof(huffmanCodes[0]));
    if (huffmanCodes == NULL) {
        return -4;
    }

    Bitset *path = InitBitset(root->height);
    build_huffman_codes(root, path, huffmanCodes, 0);
    FreeBitset(path);

    BitWriter *writer = InitBitWriter(out);
    if (writer == NULL) {
        FreeEncodeNode(root);
        for (int i = 0; i < 256; i++) {
            if (frequencies[i] != 0) {
                FreeBitset(huffmanCodes[i]);
            }
        }
        free(huffmanCodes);
        return -5;
    }

    fseek(in, pos, SEEK_SET);
    write_length(writer, nodeCount, frequencies, huffmanCodes);
    write_tree(writer, root, 0);
    write_data(writer, huffmanCodes, in);

    FlushBitWriter(writer);
    FreeBitWriter(writer);

    FreeEncodeNode(root);
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] != 0) {
            FreeBitset(huffmanCodes[i]);
        }
    }
    free(huffmanCodes);

    return 0;
}
