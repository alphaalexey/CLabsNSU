#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bad_input(void) {
    printf("bad input\n");
    exit(EXIT_SUCCESS);
}

// Function to swap two characters
void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

// Function to reverse a string
void reverse(char *ptr, char *end) {
    while (ptr < end) {
        swap(ptr, end);
        ptr++;
        end--;
    }
}

// Function to find the next permutation of a string
int next_permutation(char *str, size_t length) {
    // Find the largest index k such that str[k] < str[k+1]
    int k;
    for (k = length - 2; k >= 0; k--) {
        if (str[k] < str[k + 1]) {
            break;
        }
    }

    if (k < 0) {
        // If no such index exists, it is the last permutation
        return 0;
    }

    // Find the largest index l greater than k such that str[k] < str[l]
    int l;
    for (l = length - 1; l > k; l--) {
        if (str[k] < str[l]) {
            break;
        }
    }

    // Swap the values at indices k and l
    swap(&str[k], &str[l]);

    // Reverse the sequence from index k+1 to the end
    reverse(str + k + 1, str + length - 1);

    return 1;
}

int check_for_duplicates(const char *str) {
    uint16_t bitset = 0;
    uint8_t index;
    for (index = 0; str[index] != '\0'; index++) {
        if (str[index] < '0' || str[index] > '9') {
            return -1;
        }
        uint16_t mask = 1 << (str[index] - '0');
        if ((bitset & mask) != 0) {
            return -1;
        }
        bitset |= mask;
    }
    return 0;
}

int main(void) {
    // Read string
    char str[512] = {0};
    if (scanf("%511[^\n]\n", str) != 1) {
        return EXIT_FAILURE;
    }

    if (check_for_duplicates(str) == -1) {
        bad_input();
    }
    size_t str_length = strlen(str);

    uint64_t N;
    if (scanf("%lu", &N) != 1) {
        return EXIT_FAILURE;
    }

    // Print permutations
    for (uint64_t i = 0; i < N; i++) {
        if (!next_permutation(str, str_length)) {
            break;
        }
        printf("%s\n", str);
    }

    return EXIT_SUCCESS;
}
