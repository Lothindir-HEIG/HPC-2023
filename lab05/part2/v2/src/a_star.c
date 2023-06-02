#include "a_star.h"
#include "min_heap.h"
#include <wchar.h>
#include <likwid-marker.h>
#include <stdbool.h>

#define GRID_EMPTY 0
#define GRID_WALL 1

void reconstruct_path(Grid *grid, Position **parent_set, Position **path);
inline void check_neighbour_tab(Grid *grid, MinHeapNode current, MinHeap *open_set, bool **open_map, bool **closed_map, int **h_costs, Position **parent_set, int i, int j);
inline void check_neighbours_struct(MinHeapNode current, MinHeap *open_set, bool **open_map, bool **closed_map, int **h_costs, Position **parent_set, int i, int j, Grid_Component *neighbour);

Path_error compute_path_struct(Grid *grid, int DEBUG, Position **path)
{
    /* Variables initialization */
    MinHeap *open_set = heap_create(grid->rows * grid->cols);
    Position **parent_set = malloc(grid->rows * sizeof(Position *));
    int **h_costs = malloc(grid->rows * sizeof(int *));
    bool **closed_map = calloc(grid->rows, sizeof(bool *));
    bool **open_map = calloc(grid->rows, sizeof(bool *));
    Position end = {grid->dst->x, grid->dst->y};

    if (!open_set || !parent_set || !h_costs || !closed_map || !open_map)
    {
        free_lists(grid, open_set, parent_set, h_costs, closed_map, open_map);
        return ERROR;
    }

    int rows = grid->rows;
    int cols = grid->cols;

    for (int i = 0; i < rows; i++)
    {
        h_costs[i] = malloc(cols * sizeof(int));
        parent_set[i] = malloc(cols * sizeof(Position));
        closed_map[i] = calloc(cols, sizeof(bool));
        open_map[i] = calloc(cols, sizeof(bool));

        if (!h_costs[i] || !parent_set[i] || !closed_map[i] || !open_map[i])
        {
            free_lists(grid, open_set, parent_set, h_costs, closed_map, open_map);
            return ERROR;
        }

        /* Bithack to compute the absolute value */
        int dx = ((i - end.x) ^ ((i - end.x) >> 31)) - ((i - end.x) >> 31);
        for (int j = 0; j < cols; j++)
        {
            int dy = ((j - end.y) ^ ((j - end.y) >> 31)) - ((j - end.y) >> 31);
            h_costs[i][j] = (dx + dy) * 10;
        }
    }

    /* Move to the start node (grid->head points to the upper left corner [0,0]) */
    Grid_Component *start_node = grid->data.head;
    for (int i = 0; i < grid->src->x; i++)
        start_node = start_node->down;
    for (int i = 0; i < grid->src->y; i++)
        start_node = start_node->right;

    MinHeapNode start = {grid->src->x, grid->src->y, 0, h_costs[grid->src->x][grid->src->y], start_node};
    heap_insert(open_set, start);
    open_map[grid->src->x][grid->src->y] = true;

    LIKWID_MARKER_START("a_star_struct");

    /* Main loop */
    while (open_set->size > 0)
    {
        MinHeapNode current = heap_pop(open_set);
        open_map[current.x][current.y] = false;

        /* Path found */
        if (current.x == end.x && current.y == end.y)
        {
            reconstruct_path(grid, parent_set, path);

            LIKWID_MARKER_STOP("a_star_struct");

            if (DEBUG && path)
                print_grid(grid, *path, open_map, closed_map, true);

            free_lists(grid, open_set, parent_set, h_costs, closed_map, open_map);

            return PATH_FOUND;
        }

        closed_map[current.x][current.y] = true;

        Grid_Component *current_node = (Grid_Component *)current.data;
        check_neighbours_struct(current, open_set, open_map, closed_map, h_costs, parent_set, -1, 0, current_node->up);
        check_neighbours_struct(current, open_set, open_map, closed_map, h_costs, parent_set, 1, 0, current_node->down);
        check_neighbours_struct(current, open_set, open_map, closed_map, h_costs, parent_set, 0, -1, current_node->left);
        check_neighbours_struct(current, open_set, open_map, closed_map, h_costs, parent_set, 0, 1, current_node->right);
        // check_neighbours(current, open_set, open_map, closed_map, h_costs, parent_set);
    }

    /* Path not found */
    LIKWID_MARKER_STOP("a_star_struct");

    free_lists(grid, open_set, parent_set, h_costs, closed_map, open_map);

    return NO_PATH_FOUND;
}

Path_error compute_path_tab(Grid *grid, int DEBUG, Position **path)
{
    /* Variables initialization */
    MinHeap *open_set = heap_create(grid->rows * grid->cols);
    Position **parent_set = malloc(grid->rows * sizeof(Position *));
    int **h_costs = malloc(grid->rows * sizeof(int *));
    bool **closed_map = calloc(grid->rows, sizeof(bool *));
    bool **open_map = calloc(grid->rows, sizeof(bool *));
    Position end = {grid->dst->x, grid->dst->y};

    if (!open_set || !parent_set || !h_costs || !closed_map || !open_map)
    {
        free_lists(grid, open_set, parent_set, h_costs, closed_map, open_map);
        return ERROR;
    }

    int rows = grid->rows;
    int cols = grid->cols;

    for (int i = 0; i < rows; i++)
    {
        h_costs[i] = malloc(cols * sizeof(int));
        parent_set[i] = malloc(cols * sizeof(Position));
        closed_map[i] = calloc(cols, sizeof(bool));
        open_map[i] = calloc(cols, sizeof(bool));

        if (!h_costs[i] || !parent_set[i] || !closed_map[i] || !open_map[i])
        {
            free_lists(grid, open_set, parent_set, h_costs, closed_map, open_map);
            return ERROR;
        }

        int dx = (i - end.x) * (i - end.x);
        for (int j = 0; j < cols - 3; j += 4)
        {
            int dy = j - end.y;
            int dy1 = dy + 1;
            int dy2 = dy + 2;
            int dy3 = dy + 3;
            h_costs[i][j] = (dx + dy * dy) * 10;
            h_costs[i][j + 1] = (dx + dy1 * dy1) * 10;
            h_costs[i][j + 2] = (dx + dy2 * dy2) * 10;
            h_costs[i][j + 3] = (dx + dy3 * dy3) * 10;
        }
        /* Si cols n'est pas un multiple de 4 */
        for (int j = cols & ~3; j < cols; j++)
        {
            int dy = j - end.y;
            h_costs[i][j] = (dx + dy * dy) * 10;
        }
    }

    /* Add start node to open set */
    MinHeapNode start = {grid->src->x, grid->src->y, h_costs[grid->src->x][grid->src->y], 0, NULL};
    heap_insert(open_set, start);
    open_map[grid->src->x][grid->src->y] = true;

    LIKWID_MARKER_START("a_star_tab");

    /* Main loop */
    while (open_set->size > 0)
    {
        MinHeapNode current = heap_pop(open_set);
        open_map[current.x][current.y] = false;

        /* Path found */
        if (current.x == end.x && current.y == end.y)
        {
            reconstruct_path(grid, parent_set, path);

            LIKWID_MARKER_STOP("a_star_tab");

            if (DEBUG && path)
                print_grid(grid, *path, open_map, closed_map, false);

            free_lists(grid, open_set, parent_set, h_costs, closed_map, open_map);

            return PATH_FOUND;
        }

        closed_map[current.x][current.y] = true;

        for (int i = -1; i <= 1; i++) // rows
        {
            check_neighbour_tab(grid, current, open_set, open_map, closed_map, h_costs, parent_set, i, -1);
            check_neighbour_tab(grid, current, open_set, open_map, closed_map, h_costs, parent_set, i, 0);
            check_neighbour_tab(grid, current, open_set, open_map, closed_map, h_costs, parent_set, i, 1);

            // for (int j = -1; j <= 1; j++) // cols
            // {
            //     int neighbour_x = current.x + i;
            //     int neighbour_y = current.y + j;

            //     if (neighbour_x < 0 || neighbour_x >= grid->rows || neighbour_y < 0 || neighbour_y >= grid->cols)
            //         continue;
            //     if (closed_map[neighbour_x][neighbour_y])
            //         continue;
            //     if (grid->data.grid[neighbour_x][neighbour_y] == 1)
            //         continue;

            //     int weight = (i == 0 || j == 0) ? 10 : 14;
            //     int neighbour_g_cost = current.g_cost + weight;
            //     int neighbour_f_cost = neighbour_g_cost + h_costs[neighbour_x][neighbour_y];

            //     if (open_map[neighbour_x][neighbour_y])
            //     {
            //         int index = heap_search(open_set, neighbour_x, neighbour_y);
            //         if (open_set->data[index].f_cost > neighbour_f_cost)
            //         {
            //             parent_set[neighbour_x][neighbour_y] = (Position){current.x, current.y};
            //             open_set->data[index].g_cost = neighbour_g_cost;
            //             heap_update(open_set, index, neighbour_f_cost);
            //         }
            //     }
            //     else
            //     {
            //         parent_set[neighbour_x][neighbour_y] = (Position){current.x, current.y};
            //         MinHeapNode neighbour = {neighbour_x, neighbour_y, neighbour_g_cost, neighbour_f_cost, NULL};
            //         heap_insert(open_set, neighbour);
            //         open_map[neighbour_x][neighbour_y] = true;
            //     }
            // }
        }
    }

    /* Path not found */
    LIKWID_MARKER_STOP("a_star_tab");

    free_lists(grid, open_set, parent_set, h_costs, closed_map, open_map);

    return NO_PATH_FOUND;
}

inline void check_neighbours_struct(MinHeapNode current, MinHeap *open_set, bool **open_map, bool **closed_map, int **h_costs, Position **parent_set, int i, int j, Grid_Component *neighbour)
{
    // Check if neighbour is traversable
    if (neighbour == NULL || neighbour->value == 1)
        return;

    if (closed_map[current.x + i][current.y + j])
        return;

    int neighbour_g_cost = current.g_cost + 10;
    int neighbour_f_cost = neighbour_g_cost + h_costs[current.x + i][current.y + j];

    if (open_map[current.x + i][current.y + j])
    {
        int index = heap_search(open_set, current.x + i, current.y + j);
        if (open_set->data[index].f_cost > neighbour_f_cost)
        {
            parent_set[current.x + i][current.y + j] = (Position){current.x, current.y};
            open_set->data[index].g_cost = neighbour_g_cost;
            heap_update(open_set, index, neighbour_f_cost);
        }
    }
    else
    {
        MinHeapNode neighbour_node = {current.x + i, current.y + j, neighbour_g_cost, neighbour_f_cost, neighbour};
        parent_set[current.x + i][current.y + j] = (Position){current.x, current.y};
        heap_insert(open_set, neighbour_node);
        open_map[current.x + i][current.y + j] = true;
    }
}

inline void check_neighbour_tab(Grid *grid, MinHeapNode current, MinHeap *open_set, bool **open_map, bool **closed_map, int **h_costs, Position **parent_set, int i, int j)
{
    int neighbour_x = current.x + i;
    int neighbour_y = current.y + j;

    if (neighbour_x < 0 || neighbour_x >= grid->rows || neighbour_y < 0 || neighbour_y >= grid->cols)
        return;
    if (closed_map[neighbour_x][neighbour_y])
        return;
    if (grid->data.grid[neighbour_x][neighbour_y] == 1)
        return;

    int weight = (i == 0 || j == 0) ? 10 : 14;
    int neighbour_g_cost = current.g_cost + weight;
    int neighbour_f_cost = neighbour_g_cost + h_costs[neighbour_x][neighbour_y];

    if (open_map[neighbour_x][neighbour_y])
    {
        int index = heap_search(open_set, neighbour_x, neighbour_y);
        if (open_set->data[index].f_cost > neighbour_f_cost)
        {
            parent_set[neighbour_x][neighbour_y] = (Position){current.x, current.y};
            open_set->data[index].g_cost = neighbour_g_cost;
            heap_update(open_set, index, neighbour_f_cost);
        }
    }
    else
    {
        parent_set[neighbour_x][neighbour_y] = (Position){current.x, current.y};
        MinHeapNode neighbour = {neighbour_x, neighbour_y, neighbour_g_cost, neighbour_f_cost, NULL};
        heap_insert(open_set, neighbour);
        open_map[neighbour_x][neighbour_y] = true;
    }
}

inline void reconstruct_path(Grid *grid, Position **parent_set, Position **path)
{
    if (path)
    {
        Position *path_rec = malloc(grid->rows * grid->cols * sizeof(Position));
        int path_len = 0;

        Position current = {grid->dst->x, grid->dst->y};
        path_rec[path_len++] = current;
        while (current.x != grid->src->x || current.y != grid->src->y)
        {
            current = parent_set[current.x][current.y];
            path_rec[path_len++] = current;
        }

        path_rec = realloc(path_rec, path_len * sizeof(Position));

        *path = path_rec;
    }
}
