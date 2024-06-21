#pragma once

#include <stdio.h>

#include "bit_set.h"

typedef struct BitWriter BitWriter;

BitWriter *InitBitWriter(FILE *file);
void FreeBitWriter(BitWriter *writer);
void WriteBit(BitWriter *writer, int bit);
void WriteBits(BitWriter *writer, int data, int length);
void WriteBitSet(BitWriter *writer, Bitset *bitset);
void FlushBitWriter(BitWriter *writer);
