#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STOP_TABLE_LENGTH (UINT8_MAX + 1)

void make_stop_table(size_t stop_table[], size_t stop_table_length,
                     const uint8_t format[], size_t format_length) {
    for (size_t i = 0; i < stop_table_length; i++) {
        stop_table[i] = format_length;
    }
    for (size_t i = 0; i < format_length - 1; i++) {
        stop_table[format[i]] = format_length - i - 1;
    }
}

size_t moor_check(const size_t stop_table[], const uint8_t buffer[],
                  const uint8_t format[], size_t length, size_t offset) {
    for (size_t i = length; i > 0; i--) {
        printf("%zu ", offset + i);
        if (buffer[i - 1] != format[i - 1]) {
            return stop_table[buffer[length - 1]];
        }
    }
    return length;
}

int main(void) {
    uint8_t format[17] = {0};
    if (scanf("%16[^\n]\n", (char *)format) != 1) {
        return EXIT_FAILURE;
    }
    size_t format_length = strlen((char *)format);
    if (format_length == 0) {
        return EXIT_FAILURE;
    }

    size_t stop_table[STOP_TABLE_LENGTH];
    make_stop_table(stop_table, STOP_TABLE_LENGTH, format, format_length);

    uint8_t buffer[format_length * 2 - 1];
    if (fread(buffer, sizeof(buffer[0]), format_length - 1, stdin) <
        format_length - 1) {
        return EXIT_SUCCESS;
    }

    size_t index = 0;
    size_t skip = 0;
    size_t current_length;
    do {
        current_length = fread(buffer + format_length - 1, sizeof(buffer[0]),
                               format_length, stdin);
        for (size_t i = skip; i < current_length;) {
            i += moor_check(stop_table, buffer + i, format, format_length,
                            index + i);
            skip = i % format_length;
        }
        memmove(buffer, buffer + current_length, format_length - 1);
        index += format_length;
    } while (current_length > 0);
    printf("\n");

    return EXIT_SUCCESS;
}
