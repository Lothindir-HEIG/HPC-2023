#ifndef MIN_HEAP_H
#define MIN_HEAP_H

typedef struct MinHeapNode
{
    int x;
    int y;
    int g_cost;
    int f_cost;
    void *data;
} MinHeapNode;

typedef struct MinHeap
{
    MinHeapNode *data;
    int size;
    int capacity;
} MinHeap;

MinHeap *heap_create(int capacity);

void heap_insert(MinHeap *heap, MinHeapNode value);

MinHeapNode heap_pop(MinHeap *heap);

int heap_search(MinHeap *heap, int x, int y);

void heap_update(MinHeap *heap, int index, int new_fcost);

void heap_free(MinHeap *heap);

#endif /* MIN_HEAP_H */
