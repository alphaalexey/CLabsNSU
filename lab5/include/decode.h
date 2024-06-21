#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct DecodeNode {
    uint8_t value;
    struct DecodeNode *left, *right;
} DecodeNode;

DecodeNode *InitDecodeNode(uint8_t value);
void FreeDecodeNode(DecodeNode *node);
