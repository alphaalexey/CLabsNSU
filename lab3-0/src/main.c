#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int32_t *a, int32_t *b) {
    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int32_t arr[], uint32_t N, uint32_t i) {
    uint32_t largest = i;
    uint32_t left = 2 * i + 1;
    uint32_t right = 2 * i + 2;

    if (left < N && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < N && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(&arr[i], &arr[largest]);

        heapify(arr, N, largest);
    }
}

void heapSort(int32_t array[], uint32_t N) {
    for (uint32_t i = N / 2; i > 0; i--) {
        heapify(array, N, i - 1);
    }

    for (uint32_t i = N; i > 0; i--) {
        swap(&array[0], &array[i - 1]);

        heapify(array, i - 1, 0);
    }
}

int main(void) {
    // Read N
    uint32_t N;
    if (scanf("%u\n", &N) != 1) {
        return EXIT_FAILURE;
    }

    // Read N numbers in array
    int32_t *array = malloc(N * sizeof(*array));
    for (uint32_t i = 0; i < N; i++) {
        if (scanf("%i\n", &array[i]) != 1) {
            return EXIT_FAILURE;
        }
    }

    // Sort array
    heapSort(array, N);

    // Print sorted array
    for (uint32_t i = 0; i < N; i++) {
        printf("%i ", array[i]);
    }
    printf("\n");

    // Free memory
    free(array);

    return EXIT_SUCCESS;
}
