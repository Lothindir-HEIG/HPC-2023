#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "priority_queue.h"

priority_queue_t *priority_queue_create_heap(int (*compare_priority)(const void *, const void *))
{
    priority_queue_t *pq = aligned_alloc(16, sizeof(priority_queue_t));
    pq->size = 0;
    pq->capacity = 10;
    pq->compare_priority = compare_priority;
    pq->queue = aligned_alloc(16, sizeof(void *) * pq->capacity);
    return pq;
}

void push_heap(priority_queue_t *pq, void *data)
{
    if (pq->size == pq->capacity)
    {
        pq->capacity *= 2;
        // TODO: realloc is not aligned
        pq->queue = realloc(pq->queue, sizeof(void *) * pq->capacity);
    }
    pq->queue[pq->size] = data;
    pq->size++;
}

void swap(void **a, void **b)
{
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapify_down(priority_queue_t *pq, int index)
{
    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;
    int largest = index;
    if (left_child < pq->size && pq->compare_priority(pq->queue[left_child], pq->queue[largest]) > 0)
    {
        largest = left_child;
    }
    if (right_child < pq->size && pq->compare_priority(pq->queue[right_child], pq->queue[largest]) > 0)
    {
        largest = right_child;
    }
    if (largest != index)
    {
        swap(&pq->queue[index], &pq->queue[largest]);
        heapify_down(pq, largest);
    }
}

void prioritize_heap(priority_queue_t *pq)
{
    for (int i = pq->size / 2 - 1; i >= 0; i--)
    {
        heapify_down(pq, i);
    }
}

void *pop_heap(priority_queue_t *pq)
{
    if (pq->size == 0)
    {
        return NULL;
    }
    void *data = pq->queue[0];
    pq->size--;
    pq->queue[0] = pq->queue[pq->size];
    heapify_down(pq, 0);
    return data;
}

void destroy_heap(priority_queue_t *pq)
{
    free(pq->queue);
    free(pq);
}
