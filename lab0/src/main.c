#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRECESION 12

char chr(int v)
{
    if (v >= 0 && v <= 9) {
        return '0' + v;
    } else {
        return 'a' + v - 10;
    }
}

int val(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else {
        return -1;
    }
}

int isValidNumber(char* number, int base, size_t* dot_out, size_t* length_out)
{
    size_t length = strlen(number);
    size_t dot = length;

    for (size_t i = 0; i < length; i++) {
        if (number[i] == '.') {
            if (i == 0 || i == length - 1 || dot != length) {
                return 0;
            }
            dot = i;
        } else {
            int digit = val(number[i]);

            if (digit >= base || digit < 0) {
                return 0;
            }
        }
    }

    *dot_out = dot;
    *length_out = length;
    return 1;
}

uint64_t toInt(char* number, size_t length, int base)
{
    uint64_t result = 0;
    uint64_t power = 1;
    for (int64_t i = length - 1; i >= 0; i--) {
        result += val(number[i]) * power;
        power *= base;
    }
    return result;
}

void toFraction(char* number, size_t length, int base, uint64_t* n_out, uint64_t* d_out)
{
    uint64_t n = 0, d = 1;
    for (size_t i = 0; i < length; i++) {
        n *= base;
        n += val(number[i]);
        d *= base;
    }
    *n_out = n;
    *d_out = d;
}

int intToStr(uint64_t number, int base, char* result)
{
    int index = 0;

    // Преобразование целой части в заданную систему счисления
    do {
        int remainder = number % base;
        result[index++] = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
        number /= base;
    } while (number > 0);

    // Обратный порядок символов
    for (int i = 0, j = index - 1; i < j; i++, j--) {
        char temp = result[i];
        result[i] = result[j];
        result[j] = temp;
    }

    return index;
}

void fractionToStr(uint64_t n, uint64_t d, int base, char* result)
{
    int index = 0;
    result[index++] = '.';
    for (int i = 0; i < PRECESION && n; i++) {
        n *= base;
        result[index++] = chr(n / d);
        n %= d;
    }
}

int main(void)
{
    int base1, base2;
    if (scanf("%d %d", &base1, &base2) != 2 || base1 < 2 || base1 > 16 || base2 < 2 || base2 > 16) {
        puts("bad input\n");
        return EXIT_SUCCESS;
    }

    char number[14] = {0};
    size_t dot, length;
    if (scanf("%13s", number) != 1 || !isValidNumber(number, base1, &dot, &length)) {
        puts("bad input\n");
        return EXIT_SUCCESS;
    }

    uint64_t number1 = toInt(number, dot, base1);
    uint64_t n = 0, d;
    if (dot != length) {
        toFraction(number + dot + 1, length - dot - 1, base1, &n, &d);
    }

    char result[49] = {0};
    int count = intToStr(number1, base2, result);
    if (n) { // Если есть дробная часть, то добавляем её в строку
        fractionToStr(n, d, base2, result + count);
    }
    printf("%s\n", result);

    return EXIT_SUCCESS;
}
