#include <stdio.h>
#include <stdlib.h>

typedef enum { ITEM_TYPE_NONE, ITEM_TYPE_NUMBER, ITEM_TYPE_OPERATION } ItemType;

typedef struct {
    int value;
    ItemType type;
} Item;

void syntax_error(void) {
    puts("syntax error");
    exit(EXIT_SUCCESS);
}

void division_by_zero(void) {
    puts("division by zero");
    exit(EXIT_SUCCESS);
}

int is_number(char c) { return c >= '0' && c <= '9'; }

int is_operation(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int operation_priority(char op) {
    if (op == '*' || op == '/') {
        return 1;
    } else if (op == '+' || op == '-') {
        return 0;
    }
    return -1;
}

Item *convert_polish_notation(const char *input, size_t length,
                              size_t *stack_size, int *error) {
    size_t output_stack_size = 0;
    Item *output_stack = malloc((length + 1) * sizeof(Item));
    if (output_stack == NULL) {
        goto error_no_memory_output;
    }

    size_t operators_stack_size = 0;
    char *operators_stack = malloc(length * sizeof(char));
    if (operators_stack == NULL) {
        goto error_no_memory_operations;
    }

    size_t brackets = 0;
    size_t index = 0;
    enum ItemType {
        NONE,
        OPERATION,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        NUMBER,
    } last_item_type = NONE;
    while (input[index] != '\0') {
        if (is_number(input[index])) {
            if (last_item_type == CLOSE_BRACKET) {
                goto error_syntax_error;
            }

            int number = 0;
            while (is_number(input[index])) {
                number = number * 10 + (input[index] - '0');
                index++;
            }
            index--;

            Item item = {number, ITEM_TYPE_NUMBER};
            output_stack[output_stack_size++] = item;
            last_item_type = NUMBER;
        } else if (input[index] == '(') {
            if (last_item_type == NUMBER || last_item_type == CLOSE_BRACKET) {
                goto error_syntax_error;
            }

            brackets++;
            operators_stack[operators_stack_size++] = input[index];
            last_item_type = OPEN_BRACKET;
        } else if (input[index] == ')') {
            if (last_item_type == OPEN_BRACKET || last_item_type == OPERATION) {
                goto error_syntax_error;
            }

            if (brackets == 0) {
                goto error_syntax_error;
            }
            brackets--;
            int open_bracket = 0;
            while (operators_stack_size > 0) {
                if (operators_stack[operators_stack_size - 1] == '(') {
                    open_bracket = 1;
                    break;
                }
                Item item = {operators_stack[operators_stack_size-- - 1],
                             ITEM_TYPE_OPERATION};
                output_stack[output_stack_size++] = item;
            }
            if (open_bracket == 0) {
                goto error_syntax_error;
            }

            operators_stack_size--;
            last_item_type = CLOSE_BRACKET;
        } else if (is_operation(input[index])) {
            if (last_item_type == OPERATION || last_item_type == OPEN_BRACKET ||
                last_item_type == NONE) {
                goto error_syntax_error;
            }

            if (operators_stack_size > 0 &&
                operation_priority(input[index]) <=
                    operation_priority(
                        operators_stack[operators_stack_size - 1])) {
                Item item = {operators_stack[operators_stack_size-- - 1],
                             ITEM_TYPE_OPERATION};
                output_stack[output_stack_size++] = item;
            }
            operators_stack[operators_stack_size++] = input[index];
            last_item_type = OPERATION;
        } else {
            goto error_syntax_error;
        }
        index++;
    }

    if (last_item_type == OPERATION || brackets != 0) {
        goto error_syntax_error;
    }

    while (operators_stack_size > 0) {
        Item item = {operators_stack[operators_stack_size-- - 1],
                     ITEM_TYPE_OPERATION};
        output_stack[output_stack_size++] = item;
    }

    free(operators_stack);

    Item item = {0, ITEM_TYPE_NONE};
    output_stack[output_stack_size] = item;

    *stack_size = output_stack_size;
    *error = 0;
    return output_stack;

error_syntax_error:
    free(operators_stack);

error_no_memory_operations:
    free(output_stack);

error_no_memory_output:
    *error = 1;
    return NULL;
}

int evaluate_expression(Item stack[], size_t length, int *error) {
    size_t index = 0;
    Item *calculations = malloc(length * sizeof(calculations[0]));
    for (size_t i = 0; i < length; i++) {
        if (stack[i].type == ITEM_TYPE_NUMBER) {
            calculations[index++].value = stack[i].value;
        } else {
            int operand2 = calculations[--index].value;
            int operand1 = calculations[--index].value;
            if (stack[i].value == '*') {
                calculations[index++].value = operand1 * operand2;
            } else if (stack[i].value == '/') {
                if (operand2 == 0) {
                    goto error_division_by_zero;
                }
                calculations[index++].value = operand1 / operand2;
            } else if (stack[i].value == '+') {
                calculations[index++].value = operand1 + operand2;
            } else if (stack[i].value == '-') {
                calculations[index++].value = operand1 - operand2;
            }
        }
    }

    int ret = calculations[0].value;
    free(calculations);
    *error = 0;
    return ret;

error_division_by_zero:
    free(calculations);
    *error = 1;
    return 0;
}

int main(void) {
    char input[1001] = {0};
    if (scanf("%1000[^\n]", input) != 1) {
        syntax_error();
    }

    int error;

    size_t stack_size;
    Item *stack = convert_polish_notation(
        input, sizeof(input) / sizeof(input[0]), &stack_size, &error);
    if (error || stack[0].type == ITEM_TYPE_NONE) {
        syntax_error();
    }

    int result = evaluate_expression(stack, stack_size, &error);
    if (error) {
        division_by_zero();
    }
    printf("%d\n", result);

    free(stack);

    return EXIT_SUCCESS;
}
