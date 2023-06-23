#include <stdio.h>
#include <stdlib.h>

#include "circ_buffer.h"

int main2()
{
    // Create a circular buffer and insert 10 nodes with random costs
    circular_buffer_t *buffer = circular_buffer_create(100);
    for (int i = 0; i < 100; i++)
    {
        cb_node_t node;
        node.x = i;
        node.y = i;
        node.g_cost = rand() % 100;
        node.f_cost = rand() % 100;
        circular_buffer_insert(buffer, node);
    }

    printf("Pop the first node:\n");
    cb_node_t node = circular_buffer_pop(buffer);

    // for (int i = 0; i < 20; ++i)
    // {
    //     circular_buffer_remove(buffer);
    //     circular_buffer_insert(buffer, node);
    // }

    insertion_sort_circular_buffer(buffer);

    // Print the buffer
    printf("Buffer:\n");
    printf("Capacity: %d\n", buffer->capacity);
    printf("Front: %d\n", buffer->front);
    printf("Rear: %d\n", buffer->rear);
    printf("Size: %d\n", buffer->size);
    printf("Nodes:\n");
    for (int i = 0; i < buffer->size; i++)
    {
        cb_node_t node = circular_buffer_get_node(buffer, i);
        printf("\tNode %d: (%d, %d) g_cost: %d f_cost: %d\n", i, node.x, node.y, node.g_cost, node.f_cost);
    }
    return 0;
}