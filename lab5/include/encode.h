#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct EncodeNode {
    uint8_t value;
    int height;
    size_t frequency;
    struct EncodeNode *left, *right;
} EncodeNode;

EncodeNode *InitEncodeNode(uint8_t value, size_t frequency);
void FreeEncodeNode(EncodeNode *node);
