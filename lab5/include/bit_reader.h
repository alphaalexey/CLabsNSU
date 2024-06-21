#pragma once

#include <stdio.h>

typedef struct BitReader BitReader;

BitReader *InitBitReader(FILE *file);
void FreeBitReader(BitReader *reader);
void SetEndBits(BitReader *reader, int endBits);
int ReadBit(BitReader *reader);
int ReadBits(BitReader *reader, int length);
