#include <stdint.h>
#include <stdlib.h>

#include "bit_reader.h"

struct BitReader {
    FILE *file;
    uint8_t buffer;
    uint8_t count;
    uint8_t endBits;
    int next;
};

BitReader *InitBitReader(FILE *file) {
    BitReader *reader = malloc(sizeof(*reader));
    if (reader == NULL) {
        return NULL;
    }
    reader->file = file;
    int ch = fgetc(reader->file);
    if (ch == EOF) {
        free(reader);
        return NULL;
    }
    reader->buffer = ch;
    reader->count = 8;
    reader->next = fgetc(reader->file);
    if (reader->next == EOF) {
        free(reader);
        return NULL;
    }
    reader->endBits = 8;
    return reader;
}

void FreeBitReader(BitReader *reader) { free(reader); }

void SetEndBits(BitReader *reader, int endBits) { reader->endBits = endBits; }

int ReadBit(BitReader *reader) {
    if (reader->count == 0) {
        if (reader->next == EOF) {
            return -1;
        }
        reader->buffer = reader->next;
        reader->count = 8;
        reader->next = fgetc(reader->file);
    }

    if (reader->next == EOF && reader->count + reader->endBits <= 8) {
        return -1;
    }

    reader->count--;
    return (reader->buffer >> reader->count) & 1;
}

int ReadBits(BitReader *reader, int length) {
    int result = 0;
    for (int i = 0; i < length; i++) {
        int bit = ReadBit(reader);
        if (bit == -1) {
            return -1;
        }
        result = (result << 1) | bit;
    }
    return result;
}
