#include <stdlib.h>
#include <stdio.h>
#include "circ_buffer.h"

#define get_index(buffer, index) ((buffer)->front + (index) < (buffer)->capacity ? (buffer)->front + (index) : (buffer)->front + (index) - (buffer)->capacity)

// Function to create a circular buffer
circular_buffer_t *circular_buffer_create(size_t capacity)
{
    circular_buffer_t *buffer = (circular_buffer_t *)malloc(sizeof(circular_buffer_t));
    buffer->buffer = (cb_node_t *)malloc(sizeof(cb_node_t) * capacity);
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
void circular_buffer_insert(circular_buffer_t *buffer, cb_node_t value)
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

// Function to remove an element from the circular buffer
void circular_buffer_remove(circular_buffer_t *buffer)
{
    if (buffer->size <= 0)
    {
        // Buffer is empty
        return (cb_node_t){-1, -1, -1, -1, -1};
    }
    buffer->front = (buffer->front + 1) % buffer->capacity;
    buffer->size--;
}

cb_node_t circular_buffer_pop(circular_buffer_t *buffer)
{
    if (buffer->size <= 0)
    {
        // Buffer is empty
        return (cb_node_t){-1, -1, -1, -1, -1};
    }
    cb_node_t value = buffer->buffer[buffer->front];
    buffer->front = (buffer->front + 1) % buffer->capacity;
    buffer->size--;
    return value;
}

size_t circular_buffer_get(circular_buffer_t *buffer, int x, int y)
{
    size_t i;
    for (i = 0; i < buffer->size; i++)
    {
        if (buffer->buffer[i].x == x && buffer->buffer[i].y == y)
        {
            return i;
        }
    }
    return -1;
}

cb_node_t circular_buffer_get_node(circular_buffer_t *buffer, size_t index)
{
    return buffer->buffer[get_index(buffer, index)];
}

// Function to perform Insertion Sort on a circular buffer
void insertion_sort_circular_buffer(circular_buffer_t *buffer)
{
    int i, j;
    cb_node_t key;
    for (i = 1; i < buffer->size; i++)
    {
        key = buffer->buffer[get_index(buffer, i)];
        j = i - 1;

        // Move elements of the circular buffer, that have greater f_cost or equal f_cost but greater g_cost, to one position ahead of their current position
        while (j < buffer->rear && (buffer->buffer[get_index(buffer, j)].f_cost > key.f_cost || (buffer->buffer[get_index(buffer, j)].f_cost == key.f_cost && buffer->buffer[get_index(buffer, j)].g_cost > key.g_cost)))
        {
            buffer->buffer[(get_index(buffer, j) + 1) % buffer->capacity] = buffer->buffer[get_index(buffer, j)];
            j = (j - 1 + buffer->capacity) % buffer->capacity;
        }
        buffer->buffer[(get_index(buffer, j) + 1) % buffer->capacity] = key;
    }
}
