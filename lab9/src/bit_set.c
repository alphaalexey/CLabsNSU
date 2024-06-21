#include <stdint.h>
#include <stdlib.h>

#include "bit_set.h"

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

void FreeBitset(Bitset *bitset) { free(bitset); }

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
