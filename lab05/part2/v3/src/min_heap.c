#include <stdio.h>
#include <stdlib.h>
#include "min_heap.h"

#define PARENT(i) ((i - 1) / 2)
#define LEFT_CHILD(i) ((2 * i) + 1)
#define RIGHT_CHILD(i) ((2 * i) + 2)

MinHeap *heap_create(int capacity)
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->data = (MinHeapNode *)malloc(capacity * sizeof(MinHeapNode));
    return heap;
}

inline void swap_nodes(MinHeapNode *a, MinHeapNode *b)
{
    MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(MinHeap *heap, int index)
{
    int smallest = index;
    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;

    if (left_child < heap->size && heap->data[left_child].f_cost < heap->data[smallest].f_cost)
    {
        smallest = left_child;
    }

    if (right_child < heap->size && heap->data[right_child].f_cost < heap->data[smallest].f_cost)
    {
        smallest = right_child;
    }

    if (smallest != index)
    {
        swap_nodes(&heap->data[index], &heap->data[smallest]);
        heapify(heap, smallest);
    }
}

void heap_up(MinHeap *heap, int index)
{
    int parent_index;
    if (index > 0)
    {
        parent_index = PARENT(index);
        if (heap->data[index].f_cost < heap->data[parent_index].f_cost) // Check if current node is smaller than its parent to swap
        {
            swap_nodes(&heap->data[parent_index], &heap->data[index]);
            heap_up(heap, parent_index);
        }
    }
}

void heap_down(MinHeap *heap, int index)
{
    int left_child_index, right_child_index, min_index;
    left_child_index = LEFT_CHILD(index);
    right_child_index = RIGHT_CHILD(index);
    if (right_child_index >= heap->size)
    {
        if (left_child_index >= heap->size)
            return;
        else
            min_index = left_child_index;
    }
    else
    {
        if (heap->data[left_child_index].f_cost <= heap->data[right_child_index].f_cost) // Check which child is smaller to swap with current node
            min_index = left_child_index;
        else
            min_index = right_child_index;
    }
    if (heap->data[index].f_cost > heap->data[min_index].f_cost) // If current node is greater than smallest child, swap them and continue
    {
        swap_nodes(&heap->data[min_index], &heap->data[index]);
        heap_down(heap, min_index);
    }
}

void heap_insert(MinHeap *heap, MinHeapNode value)
{
    if (heap->size == heap->capacity)
    {
        printf("Error: Heap overflow\n");
        return;
    }

    int current_index = heap->size;
    heap->data[current_index] = value;
    heap->size++;

    while (current_index != 0 && heap->data[current_index].f_cost < heap->data[(current_index - 1) / 2].f_cost)
    {
        swap_nodes(&heap->data[current_index], &heap->data[(current_index - 1) / 2]);
        current_index = (current_index - 1) / 2;
    }
}

MinHeapNode heap_pop(MinHeap *heap)
{
    if (heap->size == 0)
    {
        printf("Error: Heap underflow\n");
        exit(1);
    }

    MinHeapNode min_node = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify(heap, 0);
    return min_node;
}

int heap_search(MinHeap *heap, int x, int y)
{
    int i;
    for (i = 0; i < heap->size; i++)
    {
        if (heap->data[i].x == x && heap->data[i].y == y)
        {
            return i; // Return the index of the value if found
        }
    }
    return -1; // Return -1 if the value is not found
}

void heap_update(MinHeap *heap, int index, int new_fcost)
{
    if (index >= 0 && index < heap->size)
    {
        if (new_fcost != heap->data[index].f_cost) // If the cost is the same do not do anything
        {
            heap->data[index].f_cost = new_fcost;
            if (heap->data[index].f_cost < heap->data[PARENT(index)].f_cost)
            {
                heap_up(heap, index);
            }
            else
            {
                heap_down(heap, index);
            }
        }
    }
}

void heap_free(MinHeap *heap)
{
    if (heap)
    {
        free(heap->data);
        free(heap);
    }
}
