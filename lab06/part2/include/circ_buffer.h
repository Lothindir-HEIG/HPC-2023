typedef struct
{
    int *buffer;
    int capacity;
    int front;
    int rear;
    int size;
} circular_buffer_t;

circular_buffer_t *circular_buffer_create(int capacity);
void circular_buffer_destroy(circular_buffer_t *buffer);
void circular_buffer_insert(circular_buffer_t *buffer, int value);
void insertion_sort_circular_buffer(circular_buffer_t *buffer);