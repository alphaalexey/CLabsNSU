#include <stdlib.h>

#include "stack.h"

typedef struct StackItem {
    size_t size;
    struct StackItem *prev;
    int *data;
} StackItem;

struct Stack {
    StackItem *end;
};

static const size_t stack_item_length = 512;

StackItem *InitStackItem(StackItem *prev) {
    StackItem *stackItem = malloc(sizeof(*stackItem));
    stackItem->prev = prev;
    stackItem->data = malloc(stack_item_length * sizeof(stackItem->data[0]));
    stackItem->size = 0;
    return stackItem;
}

Stack *InitStack(void) {
    StackItem *stackItem = InitStackItem(NULL);

    Stack *stack = malloc(sizeof(*stack));
    stack->end = stackItem;

    return stack;
}

void FreeStack(Stack *stack) {
    StackItem *end = stack->end;
    while (end) {
        StackItem *temp = end->prev;
        free(end->data);
        free(end);
        end = temp;
    }
    free(stack);
}

void PushStack(Stack *stack, int value) {
    stack->end->data[stack->end->size++] = value;
    if (stack->end->size == stack_item_length) {
        StackItem *stackItem = InitStackItem(stack->end);
        stack->end = stackItem;
    }
}

int PopStack(Stack *stack, int *value) {
    if (stack->end->size == 0) {
        if (!stack->end->prev) {
            return -1;
        }
        StackItem *temp = stack->end;
        stack->end = stack->end->prev;
        free(temp->data);
        free(temp);
    }
    stack->end->size--;
    if (value != NULL) {
        *value = stack->end->data[stack->end->size];
    }
    return 0;
}

int GetStackTop(Stack *stack, int *value) {
    StackItem *temp = stack->end;
    if (temp->size == 0) {
        if (!temp->prev) {
            return -1;
        }
        temp = temp->prev;
    }
    *value = temp->data[temp->size - 1];
    return 0;
}
