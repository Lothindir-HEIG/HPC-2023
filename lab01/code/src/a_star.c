#include "a_star.h"
#include <wchar.h>

int compute_manhattan_distance(Position *src, Position *dst);
void remove_element_and_shift(Node **list, int index, int *list_size);
void check_neighbour(Grid_Node *current, int direction, LinkedNode **open_list_head, LinkedNode **closed_list_head, Grid *grid);

Path_error compute_path_struct(Grid *grid, int DEBUG, void *path)
{
    LinkedNode *open_list = calloc(1, sizeof(LinkedNode));
    LinkedNode *closed_list = NULL;

    Grid_Node *start = calloc(1, sizeof(Grid_Node));
    start->pos.x = grid->src->x;
    start->pos.y = grid->src->y;
    start->component = grid->data.head;

    // Get from head to start node (head is at 0,0)
    for (int i = 0; i < start->pos.y; i++)
        start->component = start->component->down;
    for (int i = 0; i < start->pos.x; i++)
        start->component = start->component->right;

    open_list->node = start;

    while (open_list)
    {
        LinkedNode *current = open_list;
        LinkedNode *lowest_f_node = current;
        LinkedNode *prev = NULL;

        while (current->next)
        {
            if (current->next->node->costs.f < lowest_f_node->node->costs.f)
            {
                prev = current;
                lowest_f_node = current->next;
            }
            current = current->next;
        }

        if (prev)
        {
            if (lowest_f_node->next)
                prev->next = lowest_f_node->next;
            else
                prev->next = NULL;
        }
        else if (lowest_f_node->next)
            open_list = lowest_f_node->next;
        else
            open_list = NULL;

        // Add lowest_f_node to closed_list
        lowest_f_node->next = closed_list;
        closed_list = lowest_f_node;

        current = closed_list;

        // printf("Lowest node is (%d, %d) with f = %d\n", current->node->pos.x, current->node->pos.y, current->node->costs.f);

        if (current->node->pos.x == grid->dst->x && current->node->pos.y == grid->dst->y)
        {
            while (open_list && open_list->next)
            {
                LinkedNode *cur = open_list->next;
                free(cur->node);
                cur->node = NULL;
                if (cur->next)
                    open_list->next = cur->next;
                else
                    open_list->next = NULL;
                free(cur);
                cur = NULL;
            }
            free(open_list);

            while (closed_list && closed_list->next)
            {
                LinkedNode *cur = closed_list->next;
                free(cur->node);
                cur->node = NULL;
                if (cur->next)
                    closed_list->next = cur->next;
                else
                    closed_list->next = NULL;
                free(cur);
                cur = NULL;
            }
            free(closed_list);

            return PATH_FOUND;
        }

        check_neighbour(current->node, GRID_DIRECTION_RIGHT, &open_list, &closed_list, grid);
        check_neighbour(current->node, GRID_DIRECTION_LEFT, &open_list, &closed_list, grid);
        check_neighbour(current->node, GRID_DIRECTION_UP, &open_list, &closed_list, grid);
        check_neighbour(current->node, GRID_DIRECTION_DOWN, &open_list, &closed_list, grid);
    }

    // Free dynamically allocated memory
    if (open_list)
    {
        while (open_list->next)
        {
            LinkedNode *cur = open_list->next;
            free(cur->node);
            cur->node = NULL;
            if (cur->next)
                open_list->next = cur->next;
            else
                open_list->next = NULL;
            free(cur);
            cur = NULL;
        }
        free(open_list);
    }

    if (closed_list)
    {
        while (closed_list->next)
        {
            LinkedNode *cur = closed_list->next;
            free(cur->node);
            cur->node = NULL;
            if (cur->next)
                closed_list->next = cur->next;
            else
                closed_list->next = NULL;
            free(cur);
            cur = NULL;
        }
        free(closed_list);
    }

    return NO_PATH_FOUND;
}

Path_error compute_path_tab(Grid *grid, int DEBUG, void *path)
{
    Node **open_list = calloc(grid->rows * grid->cols, sizeof(Node *));
    Node **closed_list = calloc(grid->rows * grid->cols, sizeof(Node *));

    int open_list_index = 0;
    int closed_list_index = 0;

    Node *start = malloc(sizeof(Node));
    start->pos.x = grid->src->x;
    start->pos.y = grid->src->y;
    start->parent = NULL;

    open_list[open_list_index++] = start;

    while (open_list_index > 0)
    {
        Node *current = open_list[0];
        int current_index = 0;

        for (int i = 0; i < open_list_index; i++)
        {
            if (open_list[i]->costs.f < current->costs.f)
            {
                current = open_list[i];
                current_index = i;
            }
        }

        remove_element_and_shift(open_list, current_index, &open_list_index);
        closed_list[closed_list_index++] = current;

        if (current->pos.x == grid->dst->x && current->pos.y == grid->dst->y)
        {
            if (DEBUG)
            {
                Node **path = calloc(grid->rows * grid->cols, sizeof(Node *));
                int path_index = 0;
                while (current->parent)
                {
                    path[path_index++] = current;
                    current = current->parent;
                }

                print_grid(grid, open_list, open_list_index, closed_list, closed_list_index, path, path_index);

                free(path);
            }

            for (int i = 0; i < open_list_index; i++)
                free(open_list[i]);
            free(open_list);

            for (int i = 0; i < closed_list_index; i++)
                free(closed_list[i]);
            free(closed_list);
            return PATH_FOUND;
        }

        for (int i = -1; i <= 1; i++) // rows
        {
            for (int j = -1; j <= 1; j++) // cols
            {
                int x = current->pos.x + j;
                int y = current->pos.y + i;

                if (x < 0 || x >= grid->cols || y < 0 || y >= grid->rows)
                    continue;

                if (grid->data.grid[y][x] == 1)
                    continue;

                if (is_in_list(closed_list, closed_list_index, x, y))
                    continue;

                int weight = (abs(i) + abs(j) == 2) ? 14 : 10;
                Position neighbor_position = {current->pos.x + j, current->pos.y + i};
                int g = current->costs.g + weight;
                int h = compute_manhattan_distance(&(neighbor_position), grid->dst);
                int f = g + h;

                Node *neighbor = 0;
                if ((neighbor = is_in_list(open_list, open_list_index, x, y)))
                {
                    if (g < neighbor->costs.g)
                    {
                        neighbor->parent = current;
                        neighbor->costs.g = g;
                        neighbor->costs.f = f;
                    }
                }
                else
                {
                    Node *new_neighbor = malloc(sizeof(Node));
                    new_neighbor->pos.x = x;
                    new_neighbor->pos.y = y;
                    new_neighbor->parent = current;
                    new_neighbor->costs.g = g;
                    new_neighbor->costs.h = h;
                    new_neighbor->costs.f = f;
                    open_list[open_list_index++] = new_neighbor;
                }
            }
        }
    }

    if (DEBUG)
    {
        print_grid(grid, open_list, open_list_index, closed_list, closed_list_index, NULL, 0);
    }

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

void check_neighbour(Grid_Node *current, int direction, LinkedNode **open_list_head, LinkedNode **closed_list_head, Grid *grid)
{
    Grid_Component *neighbour;
    int i = 0, j = 0; // origin is up left [j is col/x, i is row/y]
    switch (direction)
    {
    case GRID_DIRECTION_RIGHT:
        neighbour = current->component->right;
        j = 1;
        break;
    case GRID_DIRECTION_LEFT:
        neighbour = current->component->left;
        j = -1;
        break;
    case GRID_DIRECTION_UP:
        neighbour = current->component->up;
        i = -1;
        break;
    case GRID_DIRECTION_DOWN:
        neighbour = current->component->down;
        i = 1;
        break;

    default:
        return;
    }

    // Check if neighbour is traversable
    if (neighbour == NULL || neighbour->value == 1)
        return;

    // Check is neighbour is in closed list
    LinkedNode *closed_list_node = *closed_list_head;
    while (closed_list_node)
    {
        if (closed_list_node->node->component == neighbour)
            return;
        closed_list_node = closed_list_node->next;
    }

    // printf("Checking node at (%d, %d)\n", current->pos.x + j, current->pos.y + i);

    Position neighbour_position = {current->pos.x + j, current->pos.y + i};
    int g = current->costs.g + 10;
    int h = compute_manhattan_distance(&(neighbour_position), grid->dst);
    int f = g + h;

    // Check if neighbour is in open list
    LinkedNode *open_list_node = *open_list_head;
    while (open_list_node)
    {
        if (open_list_node->node->component == neighbour)
        {
            if (g < open_list_node->node->costs.g)
            {
                open_list_node->node->parent = current;
                open_list_node->node->costs.g = g;
                open_list_node->node->costs.f = f;
            }
            return;
        }
        open_list_node = open_list_node->next;
    }

    // Add neighbour to open list
    Grid_Node *new_node = malloc(sizeof(Grid_Node));
    new_node->component = neighbour;
    new_node->parent = current;
    new_node->pos.x = current->pos.x + j;
    new_node->pos.y = current->pos.y + i;
    new_node->costs.g = g;
    new_node->costs.h = h;
    new_node->costs.f = f;

    LinkedNode *new_linked_node = malloc(sizeof(LinkedNode));
    new_linked_node->node = new_node;

    new_linked_node->next = *open_list_head;
    *open_list_head = new_linked_node;
}
