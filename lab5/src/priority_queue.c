#include <stdint.h>
#include <stdlib.h>

#include "encode.h"
#include "priority_queue.h"

struct PriorityQueue {
    int capacity;
    int length;
    EncodeNode **data;
};

PriorityQueue *InitPriorityQueue(const size_t frequencies[256]) {
    PriorityQueue *queue = malloc(sizeof(*queue));
    if (queue == NULL) {
        return NULL;
    }

    queue->capacity = 256;
    queue->length = 0;
    queue->data = malloc(256 * sizeof(queue->data[0]));

    for (int i = 0; i < 256; i++) {
        if (frequencies[i] == 0) {
            continue;
        }
        EncodeNode *node = InitEncodeNode(i, frequencies[i]);
        PriorityQueueInsert(queue, node);
    }

    return queue;
}

void FreePriorityQueue(PriorityQueue *queue) {
    free(queue->data);
    free(queue);
}

int PriorityQueueInsert(PriorityQueue *queue, EncodeNode *node) {
    if (queue->length == queue->capacity) {
        return -1;
    }

    int i = queue->length++;
    while (i > 0 && queue->data[(i - 1) / 2]->frequency > node->frequency) {
        queue->data[i] = queue->data[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    queue->data[i] = node;

    return 0;
}

void swap_nodes(EncodeNode **a, EncodeNode **b) {
    EncodeNode *tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapify_min(PriorityQueue *queue, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < queue->length &&
        queue->data[left]->frequency < queue->data[smallest]->frequency) {
        smallest = left;
    }
    if (right < queue->length &&
        queue->data[right]->frequency < queue->data[smallest]->frequency) {
        smallest = right;
    }
    if (smallest != index) {
        swap_nodes(&(queue->data[index]), &(queue->data[smallest]));
        heapify_min(queue, smallest);
    }
}

EncodeNode *PriorityQueuePop(PriorityQueue *queue) {
    if (queue->length == 0) {
        return NULL;
    }
    EncodeNode *ret = queue->data[0];
    queue->data[0] = queue->data[--queue->length];
    heapify_min(queue, 0);
    return ret;
}
