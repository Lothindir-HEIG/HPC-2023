#include "a_star.h"

int compute_manhattan_distance(Position *src, Position *dst);
void remove_element_and_shift(Node **list, int index, int *list_size);
Node *is_in_list(Node **list, int list_size, int x, int y);

Path_error compute_path_struct(Grid *grid, void *path)
{
    // create open list
    // create closed list
    // add src to open list
    // while open list is not empty
    //      get node with lowest f from open list
    //      add node to closed list
    //      if node is dst
    //          return path
    //      for each neighbor of node
    //          if neighbor is not traversable or neighbor is in closed list
    //              skip
    //          if new path to neighbor is shorter or neighbor is not in open list
    //              set f, g, h of neighbor
    //              set parent of neighbor to node
    //              if neighbor is not in open list
    //                  add neighbor to open list
    // return no path found

    // Node *open_list = malloc(sizeof(Node));
    // open_list->pos->x = grid->src->x;
    // open_list->pos->y = grid->src->y;
    // open_list->parent = NULL;
    // open_list->previous = NULL;
    // open_list->g = 0;
    // open_list->h = compute_manhattan_distance(open_list->pos, grid->dst);
    // open_list->f = open_list->h;

    // Node *closed_list;

    return NO_PATH_FOUND;
}

Path_error compute_path_tab(Grid *grid, void *path)
{
    Node **open_list = calloc(grid->rows * grid->cols, sizeof(Node *));
    Node **closed_list = calloc(grid->rows * grid->cols, sizeof(Node *));

    int open_list_index = 0;
    int closed_list_index = 0;

    Node *start = malloc(sizeof(Node));
    start->pos->x = grid->src->x;
    start->pos->y = grid->src->y;
    start->parent = NULL;

    open_list[open_list_index++] = start;
    printf("Added node (%d, %d) to open list , first node %p\n", start->pos->x, start->pos->y, (void *)open_list[0]);

    while (open_list_index > 0)
    {
        printf("Open list size: %d\n", open_list_index);
        Node *current = 0; // open_list[0];
        int current_index = 0;

        for (int i = 0; i < open_list_index; i++)
        {
            if (open_list[i]->f < current->f)
            {
                current = open_list[i];
                current_index = i;
            }
        }

        remove_element_and_shift(open_list, current_index, &open_list_index);
        closed_list[closed_list_index++] = current;

        if (current->pos->x == grid->dst->x && current->pos->y == grid->dst->y)
            return PATH_FOUND;

        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                int x = current->pos->x + i;
                int y = current->pos->y + j;
                printf("Checking node (%d, %d)\n", x, y);

                if (x < 0 || x >= grid->rows || y < 0 || y >= grid->cols)
                    continue;

                if (grid->data.grid[x][y] == 1)
                    continue;

                if (is_in_list(closed_list, closed_list_index, x, y))
                    continue;

                int g = current->g + 1;
                int h = compute_manhattan_distance(current->pos, grid->dst);
                int f = g + h;

                Node *neighbor = 0;
                if ((neighbor = is_in_list(open_list, open_list_index, x, y)))
                {
                    if (g < neighbor->g)
                    {
                        neighbor->parent = current;
                        neighbor->g = g;
                        neighbor->f = f;
                    }
                }
                else
                {
                    Node *new_neighbor = malloc(sizeof(Node));
                    new_neighbor->pos->x = x;
                    new_neighbor->pos->y = y;
                    new_neighbor->parent = current;
                    new_neighbor->g = g;
                    new_neighbor->h = h;
                    new_neighbor->f = f;
                    open_list[open_list_index++] = new_neighbor;
                    printf("Added node (%d, %d) to open list\n", x, y);
                }
            }
        }
    }

    // create open list
    // create closed list
    // add src to open list
    // while open list is not empty
    //      get node with lowest f from open list
    //      add node to closed list
    //      if node is dst
    //          return path
    //      for each neighbor of node
    //          if neighbor is not traversable or neighbor is in closed list
    //              skip
    //          if new path to neighbor is shorter or neighbor is not in open list
    //              set f, g, h of neighbor
    //              set parent of neighbor to node
    //              if neighbor is not in open list
    //                  add neighbor to open list
    // return no path found

    // Free dynamically allocated memory
    for (int i = 0; i < open_list_index; i++)
        free(open_list[i]);
    free(open_list);

    for (int i = 0; i < closed_list_index; i++)
        free(closed_list[i]);
    free(closed_list);

    return NO_PATH_FOUND;
}

int compute_manhattan_distance(Position *src, Position *dst)
{
    return abs(src->x - dst->x) + abs(src->y - dst->y);
}

void remove_element_and_shift(Node **list, int index, int *list_size)
{
    for (int i = index; i < *list_size; i++)
    {
        list[i] = list[i + 1];
    }
    *list_size -= 1;
}

Node *is_in_list(Node **list, int list_size, int x, int y)
{
    for (int i = 0; i < list_size; i++)
    {
        if (list[i]->pos->x == x && list[i]->pos->y == y)
        {
            return list[i];
        }
    }
    return 0;
}