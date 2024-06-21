#pragma once

#include <stddef.h>

typedef struct Stack Stack;

Stack *InitStack(void);
void FreeStack(Stack *stack);
void PushStack(Stack *stack, int value);
int PopStack(Stack *stack, int *value);
int GetStackTop(Stack *stack, int *value);
