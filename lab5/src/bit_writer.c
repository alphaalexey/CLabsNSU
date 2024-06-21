#include <stdint.h>
#include <stdlib.h>

#include "bit_writer.h"

struct BitWriter {
    FILE *file;
    uint8_t buffer;
    uint8_t count;
};

BitWriter *InitBitWriter(FILE *file) {
    BitWriter *writer = malloc(sizeof(*writer));
    if (writer == NULL) {
        return NULL;
    }
    writer->file = file;
    writer->buffer = 0;
    writer->count = 0;
    return writer;
}

void FreeBitWriter(BitWriter *writer) { free(writer); }

void WriteBit(BitWriter *writer, int bit) {
    writer->buffer = (writer->buffer << 1) | (bit & 1);
    writer->count++;
    if (writer->count == 8) {
        fwrite(&writer->buffer, sizeof(writer->buffer), 1, writer->file);
        writer->buffer = 0;
        writer->count = 0;
    }
}

void WriteBits(BitWriter *writer, int data, int length) {
    for (int i = 0; i < length; i++) {
        WriteBit(writer, (data >> (length - i - 1)) & 1);
    }
}

void WriteBitSet(BitWriter *writer, Bitset *bitset) {
    for (size_t i = 0; i < GetBitsetLength(bitset); i++) {
        WriteBit(writer, GetBitsetBit(bitset, i));
    }
}

void FlushBitWriter(BitWriter *writer) {
    if (writer->count > 0) {
        writer->buffer <<= (8 - writer->count);
        writer->count = 0;
        fwrite(&writer->buffer, sizeof(writer->buffer), 1, writer->file);
    }
}
