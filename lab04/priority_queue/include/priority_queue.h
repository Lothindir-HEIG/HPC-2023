#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    void **queue;
    int size;
    int capacity;
    int (*compare_priority)(const void *, const void *);
} priority_queue_t;

priority_queue_t *priority_queue_create(int (*compare_priority)(const void *, const void *));

void push(priority_queue_t *pq, void *data);

void prioritize(priority_queue_t *pq);

void *pop(priority_queue_t *pq);

void destroy(priority_queue_t *pq);