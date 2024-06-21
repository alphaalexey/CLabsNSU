#include <stdint.h>
#include <stdlib.h>

#include "bit_set.h"

struct Bitset {
    size_t maxLength;
    uint8_t data[];
};

Bitset *InitBitset(size_t length) {
    Bitset *bitset = NULL;
    size_t sizeInBytes = length / 8 + (length % 8 != 0);
    size_t dataSize = sizeInBytes / sizeof(bitset->data[0]) +
                      (sizeInBytes % sizeof(bitset->data[0]) != 0);
    bitset = calloc(1, sizeof(*bitset) + dataSize);
    bitset->maxLength = sizeInBytes * 8;
    return bitset;
}

void FreeBitset(Bitset *bitset) { free(bitset); }

int GetBitsetBit(Bitset *bitset, size_t bitIndex) {
    if (bitIndex >= bitset->maxLength) {
        return -1;
    }
    return (bitset->data[bitIndex / 8] >> (bitIndex % 8)) & 1;
}

int SetBitsetBit(Bitset *bitset, size_t bitIndex) {
    if (bitIndex >= bitset->maxLength) {
        return -1;
    }
    bitset->data[bitIndex / 8] |= 1 << (bitIndex % 8);
    return 0;
}

// int ResetBitsetBit(Bitset *bitset, size_t bitIndex) {
//     if (bitIndex >= bitset->maxLength) {
//         return -1;
//     }
//     bitset->data[bitIndex / 8] &= ~(1 << (bitIndex % 8));
//     return 0;
// }
