#pragma once

#include <stddef.h>

typedef struct Bitset Bitset;

Bitset *InitBitset(size_t length);
void FreeBitset(Bitset *bitset);
int GetBitsetBit(Bitset *bitset, size_t index);
int SetBitsetBit(Bitset *bitset, size_t index);
