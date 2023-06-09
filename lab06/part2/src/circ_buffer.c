#include <stdlib.h>
#include "circ_buffer.h"

// Function to create a circular buffer
circular_buffer_t *circular_buffer_create(int capacity)
{
    circular_buffer_t *buffer = (circular_buffer_t *)malloc(sizeof(circular_buffer_t));
    buffer->buffer = (int *)malloc(sizeof(int) * capacity);
    buffer->capacity = capacity;
    buffer->front = 0;
    buffer->rear = -1;
    buffer->size = 0;
    return buffer;
}

// Function to destroy a circular buffer
void circular_buffer_destroy(circular_buffer_t *buffer)
{
    free(buffer->buffer);
    free(buffer);
}

// Function to insert an element into the circular buffer
void circular_buffer_insert(circular_buffer_t *buffer, int value)
{
    if (buffer->size >= buffer->capacity)
    {
        // Buffer is full
        return;
    }
    buffer->rear = (buffer->rear + 1) % buffer->capacity;
    buffer->buffer[buffer->rear] = value;
    buffer->size++;
}

// Function to perform Insertion Sort on a circular buffer
void insertion_sort_circular_buffer(circular_buffer_t *buffer)
{
    int i, key, j;
    for (i = 1; i < buffer->size; i++)
    {
        key = buffer->buffer[i];
        j = i - 1;

        // Move elements of the circular buffer, that are greater than key, to one position ahead of their current position
        while (j >= 0 && buffer->buffer[j] > key)
        {
            buffer->buffer[(j + 1) % buffer->capacity] = buffer->buffer[j];
            j = (j - 1 + buffer->capacity) % buffer->capacity;
        }
        buffer->buffer[(j + 1) % buffer->capacity] = key;
    }
}
