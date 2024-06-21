#include <stdint.h>
#include <stdlib.h>

#include "bit_reader.h"
#include "decode.h"
#include "huffman.h"

DecodeNode *InitDecodeNode(uint8_t value) {
    DecodeNode *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void FreeDecodeNode(DecodeNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->left != NULL) {
        FreeDecodeNode(node->left);
    }
    if (node->right != NULL) {
        FreeDecodeNode(node->right);
    }
    free(node);
}

static int read_length(BitReader *reader) {
    int length = 0;
    for (int i = 0; i < 3; i++) {
        int bit = ReadBit(reader);
        if (bit == -1) {
            return -1;
        }
        length = (length << 1) | bit;
    }
    return length;
}

static DecodeNode *read_tree(BitReader *reader) {
    int bit = ReadBit(reader);
    if (bit == -1) {
        return NULL;
    }
    if (bit == 0) {
        DecodeNode *left = read_tree(reader);
        if (left == NULL) {
            return NULL;
        }
        DecodeNode *right = read_tree(reader);
        if (right == NULL) {
            FreeDecodeNode(left);
            return NULL;
        }
        DecodeNode *node = InitDecodeNode(0);
        node->left = left;
        node->right = right;
        return node;
    }
    return InitDecodeNode(ReadBits(reader, 8));
}

static void parse_data(BitReader *reader, FILE *out, DecodeNode *root) {
    DecodeNode *node = root;
    for (;;) {
        int bit = ReadBit(reader);
        if (bit == -1) {
            break;
        }
        if (node->left != NULL && node->right != NULL) {
            if (bit == 0) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        if (node->left == NULL && node->right == NULL) {
            fputc(node->value, out);
            node = root;
        }
    }
}

int Decode(FILE *in, FILE *out) {
    (void)out;
    BitReader *reader = InitBitReader(in);
    if (reader == NULL) {
        return 0;
    }

    int endBits = read_length(reader);
    if (endBits == -1) {
        FreeBitReader(reader);
        return -2;
    }
    SetEndBits(reader, endBits ? endBits : 8);

    DecodeNode *root = read_tree(reader);
    if (root == NULL) {
        FreeBitReader(reader);
        return -3;
    }
    parse_data(reader, out, root);
    FreeDecodeNode(root);

    FreeBitReader(reader);

    return 0;
}
