#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t mpow(uint64_t base, uint64_t e) {
    uint64_t answer = 1;
    for (uint64_t i = 0; i < e; i++) {
        answer *= base;
    }
    return answer;
}

/* Calculate the hash of a buffer */
uint64_t hash(const uint8_t buffer[], size_t length) {
    uint64_t result = 0;
    uint64_t multiplier = 1;
    for (size_t i = 0; i < length; i++) {
        result += (buffer[i] % 3) * multiplier;
        multiplier *= 3;
    }
    return result;
}

/* Update the hash */
uint64_t update_hash(uint64_t old_hash, uint8_t old_byte, uint8_t new_byte,
                     uint64_t pow_value) {
    old_hash -= old_byte % 3;
    return old_hash - (old_hash / 3) * 2 + (new_byte % 3) * pow_value;
}

int main(void) {
    /* Format */
    uint8_t format[17] = {0};
    if (scanf("%16[^\n]\n", (char *)format) != 1) {
        return EXIT_FAILURE;
    }
    size_t format_length = strlen((char *)format);
    uint64_t format_hash = hash(format, format_length);
    printf("%lu ", format_hash);

    /* Buffer */
    uint8_t buffer[format_length * 2];
    if (fread(buffer + 1, sizeof(buffer[0]), format_length - 1, stdin) <
        format_length - 1) {
        return EXIT_SUCCESS;
    }
    uint64_t buffer_hash = 0;

    uint64_t pow_value = mpow(3, format_length - 1);
    uint64_t index = 0;
    size_t current_length;
    do {
        current_length = fread(buffer + format_length, sizeof(buffer[0]),
                               format_length, stdin);
        for (size_t i = 0; i < current_length; i++) {
            buffer_hash =
                buffer_hash ? update_hash(buffer_hash, buffer[i],
                                          buffer[i + format_length], pow_value)
                            : hash(buffer + i + 1, format_length);
            if (buffer_hash == format_hash) {
                for (size_t j = 0; j < format_length; j++) {
                    printf("%zu ", index + i + j + 1);
                    if (buffer[i + 1 + j] != format[j]) {
                        break;
                    }
                }
            }
        }
        memmove(buffer, buffer + format_length, format_length);
        index += format_length;
    } while (current_length > 0);
    printf("\n");

    return EXIT_SUCCESS;
}
