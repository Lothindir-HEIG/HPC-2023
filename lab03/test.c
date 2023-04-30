#define PARENT(i) ((i - 1) / 2)

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

inline void swap_nodes(MinHeapNode *a, MinHeapNode *b)
{
    MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

extern void swap_nodes(MinHeapNode *a, MinHeapNode *b);

void heap_up(MinHeap *heap, int index)
{
    int parent_index;
    MinHeapNode temp;
    if (index > 0)
    {
        parent_index = PARENT(index);
        if (heap->data[index].f_cost < heap->data[parent_index].f_cost)
        {
            swap_nodes(&heap->data[parent_index], &heap->data[index]);
            heap_up(heap, parent_index);
        }
    }
}

void heap_down(MinHeap *heap, int index)
{
    int left_child_index, right_child_index, min_index;
    /* ... */
    if (heap->data[index].f_cost > heap->data[min_index].f_cost)
    {
        swap_nodes(&heap->data[min_index], &heap->data[index]);
        heap_down(heap, min_index);
    }
}

void heap_down_inline(MinHeap *heap, int index)
{
    int left_child_index, right_child_index, min_index;
    MinHeapNode temp;
    /* ... */
    if (heap->data[index].f_cost > heap->data[min_index].f_cost)
    {
        temp = heap->data[min_index];
        heap->data[min_index] = heap->data[index];
        heap->data[index] = temp;
        heap_down_inline(heap, min_index);
    }
}

int main()
{
    return 0;
}