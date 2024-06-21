#pragma once

#include <stddef.h>

typedef struct Bitset Bitset;

Bitset *InitBitset(size_t length);
Bitset *CopyBitset(Bitset *bitset, size_t length);
void FreeBitset(Bitset *bitset);
size_t GetBitsetLength(const Bitset *bitset);
int GetBitsetBit(Bitset *bitset, size_t index);
int SetBitsetBit(Bitset *bitset, size_t index);
int ResetBitsetBit(Bitset *bitset, size_t index);
