#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 512

size_t *prefixFunction(const uint8_t *str, size_t length) {
    size_t *p = malloc(length * sizeof(p[0]));
    p[0] = 0;
    for (size_t i = 1; i < length; i++) {
        int k = p[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = p[k - 1];
        }
        if (str[i] == str[k]) {
            k++;
        }
        p[i] = k;
    }
    return p;
}

size_t printKMP(const size_t *prefixes, const uint8_t *format,
                const uint8_t *buffer, size_t length, size_t offset,
                size_t needle_length, FILE *out) {
    for (size_t i = 0; i < length; i++) {
        // We found part of the pattern and the next symbol is not the same
        if (needle_length > 0 && buffer[i] != format[needle_length]) {
            fprintf(out, "%zu %zu ", offset + i - needle_length + 1,
                    needle_length);
        }
        while (needle_length > 0 && buffer[i] != format[needle_length]) {
            needle_length = prefixes[needle_length - 1];
        }
        if (buffer[i] == format[needle_length]) {
            needle_length++;
        }
    }
    return needle_length;
}

void printKMPtail(const size_t *prefixes, const uint8_t *format,
                  size_t format_length, const uint8_t *buffer, size_t length,
                  size_t offset, size_t needle_length, FILE *out) {
    for (size_t i = 0; i < length; i++) {
        // We found part of the pattern and the next symbol is not the same
        // AND
        // The pattern can theoretically fit in the buffer
        if (needle_length > 0 && buffer[i] != format[needle_length] &&
            i - needle_length + 1 + format_length <= length) {
            fprintf(out, "%zu %zu ", offset + i - needle_length + 1,
                    needle_length);
        }
        while (needle_length > 0 && buffer[i] != format[needle_length]) {
            needle_length = prefixes[needle_length - 1];
        }
        if (buffer[i] == format[needle_length]) {
            needle_length++;
        }
    }
    // Workaround for situations when the pattern is in the end of the buffer
    if (format_length == needle_length) {
        fprintf(out, "%zu %zu ", offset + length - needle_length + 1,
                needle_length);
    }
}

void printKMPBuffered(const size_t *prefixes, const uint8_t *format,
                      size_t format_length, FILE *in, FILE *out) {
    char *buffer = malloc(BUFFER_SIZE);
    size_t needle_length = 0, offset = 0, length;
    // Read format_length - 1 bytes if the pattern length allows it
    if (format_length > 1 && fread(buffer, sizeof(buffer[0]), format_length - 1,
                                   in) < format_length - 1) {
        free(buffer);
        return;
    }
    // Each iteration we add to buffer BUFLEN - format_length + 1 bytes.
    // We work with first BUFLEN - format_length + 1 bytes or less if
    // we read less. But we guarantee that we work with at least 1 byte and that
    // tail check function will take buffer of length format_length - 1.
    // It also can be zero length but the function should work it out anyway
    while ((length = fread(buffer + format_length - 1, sizeof(buffer[0]),
                           BUFFER_SIZE - format_length + 1, in)) > 0) {
        needle_length = printKMP(prefixes, format, (uint8_t *)buffer, length,
                                 offset, needle_length, out);
        offset += length;
        memmove(buffer, buffer + length, format_length - 1);
    }
    printKMPtail(prefixes, format, format_length, (uint8_t *)buffer + length,
                 format_length - 1, offset, needle_length, out);
    fprintf(out, "\n");

    free(buffer);
}

int main(void) {
    FILE *in = stdin;
    FILE *out = stdout;

    char format[17] = {0};
    if (fscanf(in, "%16[^\n]\n", format) != 1) {
        return EXIT_SUCCESS;
    }
    size_t format_length = strlen(format);
    if (format_length == 0) {
        return EXIT_SUCCESS;
    }

    size_t *prefixes = prefixFunction((uint8_t *)format, format_length);
    for (size_t i = 0; i < format_length; i++) {
        fprintf(out, "%zu ", prefixes[i]);
    }
    fprintf(out, "\n");

    printKMPBuffered(prefixes, (uint8_t *)format, format_length, in, out);

    free(prefixes);
    return EXIT_SUCCESS;
}
