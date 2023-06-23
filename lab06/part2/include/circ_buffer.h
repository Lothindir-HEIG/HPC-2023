#ifndef CIRC_BUFFER_H
#define CIRC_BUFFER_H

typedef struct cb_node_t
{
    int x;
    int y;
    int g_cost;
    int f_cost;
    void *data;
} cb_node_t;

typedef struct circular_buffer_t
{
    cb_node_t *buffer;
    size_t capacity;
    size_t front;
    size_t rear;
    size_t size;
} circular_buffer_t;

/**
 * @brief Create a circular buffer object with the given capacity
 * @return circular_buffer_t* Pointer to the created circular buffer
 */
circular_buffer_t *circular_buffer_create(size_t capacity);

/**
 * @brief Destroy the given circular buffer object
 * @param buffer Pointer to the circular buffer to destroy
 */
void circular_buffer_destroy(circular_buffer_t *buffer);

/**
 * @brief Insert the given value into the circular buffer
 * @param buffer Pointer to the circular buffer to insert into
 * @param value Value to insert into the circular buffer
 */
void circular_buffer_insert(circular_buffer_t *buffer, cb_node_t value);

/**
 * @brief Remove the first value from the circular buffer
 * @param buffer Pointer to the circular buffer to remove from
 */
void circular_buffer_remove(circular_buffer_t *buffer);

/**
 * @brief Pop the first value from the circular buffer
 * @param buffer Pointer to the circular buffer to pop from
 * @return cb_node_t The first value from the circular buffer
 */
cb_node_t circular_buffer_pop(circular_buffer_t *buffer);

/**
 * @brief Get the index of the given value in the circular buffer
 * @param buffer Pointer to the circular buffer to search
 * @param x X coordinate of the value to search for
 * @param y Y coordinate of the value to search for
 * @return size_t Index of the given value in the circular buffer
 */
size_t circular_buffer_get(circular_buffer_t *buffer, int x, int y);

/**
 * @brief Get the value at the given index in the circular buffer
 * @param buffer Pointer to the circular buffer to get from
 * @param index Index of the value to get
 * @return cb_node_t Value at the given index in the circular buffer
 */
cb_node_t circular_buffer_get_node(circular_buffer_t *buffer, size_t index);

/**
 * @brief Sort the buffer using insertion sort
 * @param buffer Pointer to the circular buffer to sort
 */
void insertion_sort_circular_buffer(circular_buffer_t *buffer);

#endif /* CIRC_BUFFER_H */