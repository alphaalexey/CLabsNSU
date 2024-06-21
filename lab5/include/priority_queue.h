#pragma once

#include <stddef.h>

#include "encode.h"

typedef struct PriorityQueue PriorityQueue;

PriorityQueue *InitPriorityQueue(const size_t frequencies[256]);
void FreePriorityQueue(PriorityQueue *queue);
int PriorityQueueInsert(PriorityQueue *queue, EncodeNode *node);
EncodeNode *PriorityQueuePop(PriorityQueue *queue);
