#include "priority_queue.h"
#include <immintrin.h>

/**
 * @brief Create a priority queue object
 *
 * @param compare_priority Function pointer to compare the priority of two elements
 * @return priority_queue_t* Pointer to the priority queue object
 */
priority_queue_t *priority_queue_create(int (*compare_priority)(const void *, const void *))
{
    priority_queue_t *pq = aligned_alloc(16, sizeof(priority_queue_t));
    pq->size = 0;
    pq->capacity = 16;
    pq->compare_priority = compare_priority;
    pq->queue = aligned_alloc(16, sizeof(void *) * pq->capacity);
    return pq;
}

/**
 * @brief Push an element to the priority queue
 *
 * @param pq Pointer to the priority queue object
 * @param data Pointer to the data to be pushed
 */
void push(priority_queue_t *pq, void *data)
{
    if (pq->size == pq->capacity)
    {
        pq->capacity *= 2;
        pq->queue = realloc(pq->queue, sizeof(void *) * pq->capacity);
    }
    pq->queue[pq->size] = data;
    pq->size++;
}

/**
 * @brief Prioritize the elements in the priority queue
 * This function uses bubble sort to prioritize the elements in the priority queue.
 * The elements with the highest priority will be at the back of the queue.
 *
 * @param pq Pointer to the priority queue object
 */
void prioritize(priority_queue_t *pq)
{
    // Basic bubble sort
    for (int i = 0; i < pq->size; i++)
    {
        for (int j = 0; j < pq->size - i - 1; j++)
        {
            if (pq->compare_priority(pq->queue[j], pq->queue[j + 1]) < 0)
            {
                // Load the data of the two elements to be swapped into a SSE register
                __m128i a = _mm_loadu_si128((__m128i *)&pq->queue[j]);
                // Swap the two elements using a mask and store the result back to the queue
                _mm_storeu_si128((__m128i *)&pq->queue[j], _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
            }
        }
    }
}

/**
 * @brief Pop the element with the highest priority
 *
 * @param pq Pointer to the priority queue object
 * @return void* Pointer to the data of the element with the highest priority
 */
void *pop(priority_queue_t *pq)
{
    if (pq->size == 0)
    {
        return NULL;
    }
    void *data = pq->queue[pq->size - 1];
    pq->size--;
    return data;
}

/**
 * @brief Destroy the priority queue object
 *
 * @param pq Pointer to the priority queue object
 */
void destroy(priority_queue_t *pq)
{
    free(pq->queue);
    free(pq);
}
