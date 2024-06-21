#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int get_values(int length, int* array) {
    long long tmp;
    for (int i = 0; i < length; i++) {
        if (scanf("%lld", &tmp) < 1 || tmp < INT_MIN || tmp > INT_MAX) {
            return 1;
        }
        array[i] = (int)tmp;
    }
    return 0;
}

long long sum_array(int length, int* array) {
    long long int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += array[i];
    }
    return sum;
}

int overflows_int(long long num) {
    return num < INT_MIN || num > INT_MAX;
}

int main(void) {
    // read second line length
    int length;
    if (scanf("%d", &length) != 1 || length < 1 || length > 10) {
        printf("bad input\n");
        return EXIT_SUCCESS; // should be EXIT_FAILURE
    }

    // read array
    int array[10] = { 0 };
    if (get_values(length, array)) {
        printf("bad input\n");
        return EXIT_SUCCESS; // should be EXIT_FAILURE
    }

    // count sum
    long long sum = sum_array(length, array);
    if (overflows_int(sum)) {
        printf("overflow\n");
        return EXIT_SUCCESS; // should be EXIT_FAILURE
    }
    int sum_int = sum;
    printf("%d\n", sum_int);

    return EXIT_SUCCESS;
}
