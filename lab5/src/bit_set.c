#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bit_set.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

struct Bitset {
    size_t length;
    uint8_t data[];
};

Bitset *InitBitset(size_t length) {
    Bitset *bitset = NULL;
    size_t dataSize = length / 8 + (length % 8 != 0);
    bitset = calloc(1, sizeof(*bitset) + dataSize);
    bitset->length = length;
    return bitset;
}

Bitset *CopyBitset(Bitset *bitset, size_t length) {
    if (bitset == NULL) {
        return NULL;
    }
    Bitset *copy = InitBitset(length);
    size_t minLength = MIN(bitset->length, length);
    memcpy(copy->data, bitset->data, minLength / 8 + (minLength % 8 != 0));
    return copy;
}

void FreeBitset(Bitset *bitset) { free(bitset); }

size_t GetBitsetLength(const Bitset *bitset) { return bitset->length; }

int GetBitsetBit(Bitset *bitset, size_t bitIndex) {
    if (bitIndex >= bitset->length) {
        return -1;
    }
    return (bitset->data[bitIndex / 8] >> (bitIndex % 8)) & 1;
}

int SetBitsetBit(Bitset *bitset, size_t bitIndex) {
    if (bitIndex >= bitset->length) {
        return -1;
    }
    bitset->data[bitIndex / 8] |= 1 << (bitIndex % 8);
    return 0;
}

int ResetBitsetBit(Bitset *bitset, size_t bitIndex) {
    if (bitIndex >= bitset->length) {
        return -1;
    }
    bitset->data[bitIndex / 8] &= ~(1 << (bitIndex % 8));
    return 0;
}
