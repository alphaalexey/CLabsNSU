#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int32_t *a, int32_t *b) {
    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(int32_t array[], uint32_t length) {
    int32_t left = 0;
    int32_t right = length - 1;
    int32_t pivot = array[right / 2];
    while (left <= right) {
        while (pivot > array[left]) {
            left++;
        }
        while (pivot < array[right]) {
            right--;
        }
        if (left <= right) {
            swap(&array[left], &array[right]);
            left++;
            right--;
        }
    }
    if (right > 0) {
        quickSort(array, right + 1);
    }
    if (length - left > 1) {
        quickSort(array + left, length - left);
    }
}

void printArray(int32_t array[], uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        printf("%i ", array[i]);
    }
    printf("\n");
}

int main(void) {
    uint32_t N;
    if (scanf("%u\n", &N) != 1) {
        return EXIT_FAILURE;
    }
    if (N == 0) {
        return EXIT_SUCCESS;
    }

    int32_t *array = malloc(N * sizeof(*array));
    for (uint32_t i = 0; i < N; i++) {
        if (scanf("%i\n", &array[i]) != 1) {
            return EXIT_FAILURE;
        }
    }

    quickSort(array, N);

    printArray(array, N);

    free(array);

    return EXIT_SUCCESS;
}
