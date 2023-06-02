/* Bruno Da Rocha Carvalho / REDS / HEIG-VD
 * HPC - 24.02.23
 * utils.c
 *
 * Utilitaire pour la création de grille.
 */

#include "utils.h"

int **init_grid_tab(int rows, int columns, int DEBUG, Position *src, Position *dst, char *input_file)
{
    FILE *file = fopen(input_file, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }
    int nb_rows = rows, nb_columns = columns;
    int **grid = malloc(nb_rows * sizeof(int *));
    for (int i = 0; i < nb_rows; i++)
    {
        grid[i] = malloc(nb_columns * sizeof(int));
    }

    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_columns; j++)
        {
            char c;
            int val;
            fscanf(file, " %c", &c);

            if (c == 'A')
            {
                src->x = j;
                src->y = i;
                val = 0;
            }
            else if (c == 'B')
            {
                dst->x = j;
                dst->y = i;
                val = 0;
            }
            else
            {
                val = (int)c - 48;
            }

            grid[i][j] = val;
        }
    }

    if (DEBUG)
    {
        printf("Contenu du tableau :\n");
        for (int i = 0; i < nb_rows; i++)
        {
            for (int j = 0; j < nb_columns; j++)
            {
                if (i == src->y && j == src->x)
                    printf("S ");
                else if (i == dst->y && j == dst->x)
                    printf("D ");
                else
                    printf("%d ", grid[i][j]);
            }
            printf("\n");
        }
    }

    fclose(file);

    return grid;
}

Grid_Component *init_grid_struct(int rows, int cols, int DEBUG, Position *src, Position *dst, char *input_file)
{
    FILE *fp = fopen(input_file, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Unable to open file %s", "input.txt");
        exit(1);
    }

    Grid_Component *head = NULL;
    Grid_Component *curr = NULL;
    Grid_Component *prev_row[cols];

    for (int i = 0; i < cols; i++)
    {
        prev_row[i] = NULL;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char c;
            int val;
            fscanf(fp, " %c", &c);
            if (c == 'A')
            {
                src->x = j;
                src->y = i;
                val = 0;
            }
            else if (c == 'B')
            {
                dst->x = j;
                dst->y = i;
                val = 0;
            }
            else
            {
                val = (int)c - 48;
            }
            Grid_Component *node = malloc(sizeof(Grid_Component));
            node->value = val;
            node->left = NULL;
            node->right = NULL;
            node->up = NULL;
            node->down = NULL;

            if (j == 0)
            {
                if (head == NULL)
                {
                    head = node;
                }
                curr = node;
            }
            else
            {
                curr->right = node;
                node->left = curr;
                curr = curr->right;
            }

            if (prev_row[j] != NULL)
            {
                prev_row[j]->down = node;
                node->up = prev_row[j];
            }

            prev_row[j] = node;
        }
    }

    if (DEBUG)
    {
        Grid_Component *curr_row = head;

        for (int i = 0; i < rows; i++)
        {
            Grid_Component *curr = curr_row;
            for (int j = 0; j < cols; j++)
            {
                printf("%d ", curr->value);
                curr = curr->right;
            }
            printf("\n");
            curr_row = curr_row->down;
        }
    }

    fclose(fp);
    return head;
}

void print_tab(Grid *grid, char **grid_tab)
{
    for (int i = 0; i < grid->rows; i++)
    {
        printf("│ ");
        for (int j = 0; j < grid->cols; j++)
        {
            char cell = grid_tab[i][j];
            if (cell == 'S')
                printf("%sS%s ", ANSI_COLOR_GRN, ANSI_COLOR_RST);
            else if (cell == 'D')
                printf("%sD%s ", ANSI_COLOR_RED, ANSI_COLOR_RST);
            else if (cell == 'P')
                printf("%s●%s ", ANSI_COLOR_YEL, ANSI_COLOR_RST);
            else if (cell == 'O')
                printf("%s⬡%s ", ANSI_COLOR_CYN, ANSI_COLOR_RST);
            else if (cell == 'C')
                printf("%s⬢%s ", ANSI_COLOR_BLU, ANSI_COLOR_RST);
            else if (grid->data.grid[i][j] == 1)
                printf("■ ");
            else
                printf("- ");
        }
        printf("│\n");
    }
}

void print_struct(Grid *grid, char **grid_tab)
{
    Grid_Component *curr_row = grid->data.head;

    for (int i = 0; i < grid->rows; i++)
    {
        Grid_Component *curr = curr_row;
        printf("│ ");
        for (int j = 0; j < grid->cols; j++)
        {
            char cell = grid_tab[i][j];
            if (cell == 'S')
                printf("%sS%s ", ANSI_COLOR_GRN, ANSI_COLOR_RST);
            else if (cell == 'D')
                printf("%sD%s ", ANSI_COLOR_RED, ANSI_COLOR_RST);
            else if (cell == 'P')
                printf("%s●%s ", ANSI_COLOR_YEL, ANSI_COLOR_RST);
            else if (cell == 'O')
                printf("%s⬡%s ", ANSI_COLOR_CYN, ANSI_COLOR_RST);
            else if (cell == 'C')
                printf("%s⬢%s ", ANSI_COLOR_BLU, ANSI_COLOR_RST);
            else if (curr->value == 1)
                printf("■ ");
            else
                printf("- ");

            curr = curr->right;
        }
        printf("│\n");

        curr_row = curr_row->down;
    }
}

void print_grid(Grid *grid, Position *path, bool **open_map, bool **closed_map, bool is_struct)
{
    char **grid_tab = calloc(grid->rows, sizeof(char *));
    if (grid_tab == NULL)
    {
        fprintf(stderr, "Error while trying to print the grid (bad memory allocation)\n");
        return;
    }

    for (int i = 0; i < grid->rows; i++)
    {
        grid_tab[i] = calloc(grid->cols, sizeof(char));
        if (grid_tab[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(grid_tab[j]);
            free(grid_tab);
            fprintf(stderr, "Error while trying to print the grid (bad memory allocation)\n");
            return;
        }
    }

    if (closed_map && open_map)
    {
        for (int i = 0; i < grid->rows; i++)
        {
            for (int j = 0; j < grid->cols; j++)
            {
                if (open_map[i][j])
                    grid_tab[i][j] = 'O';
                else if (closed_map[i][j])
                    grid_tab[i][j] = 'C';
            }
        }
    }

    if (path)
    {
        Position *curr = path;
        while (curr && curr->x != grid->src->x && curr->y != grid->src->y)
        {
            grid_tab[curr->x][curr->y] = 'P';
            curr++;
        }
        grid_tab[curr->x][curr->y] = 'P';
    }

    grid_tab[grid->src->x][grid->src->y] = 'S';
    grid_tab[grid->dst->x][grid->dst->y] = 'D';

    printf("Contenu de la grille :\n");

    printf("╭─");
    for (int c = 0; c < grid->cols; c++)
        printf("──");
    printf("╮\n");

    if (is_struct)
        print_struct(grid, grid_tab);
    else
        print_tab(grid, grid_tab);

    printf("╰─");
    for (int c = 0; c < grid->cols; c++)
        printf("──");
    printf("╯\n");

    for (int i = 0; i < grid->rows; i++)
    {
        free(grid_tab[i]);
    }
    free(grid_tab);
}

inline void free_list(Grid *grid, void **list)
{
    for (int i = 0; i < grid->rows; i++)
        free(list[i]);
    free(list);
}

void free_lists(Grid *grid, MinHeap *open_set, Position **parent_set, int **h_costs, bool **open_map, bool **closed_map)
{
    heap_free(open_set);
    for (int i = 0; i < grid->rows; i++)
    {
        free(parent_set[i]);
    }
    for (int i = 0; i < grid->rows; i++)
    {
        free(h_costs[i]);
    }
    for (int i = 0; i < grid->rows; i++)
    {
        free(open_map[i]);
    }
    for (int i = 0; i < grid->rows; i++)
    {
        free(closed_map[i]);
    }
    // for (int i = 0; i < grid->rows; i++)
    // {
    //     free(parent_set[i]);
    //     free(h_costs[i]);
    //     free(open_map[i]);
    //     free(closed_map[i]);
    // }
    free(parent_set);
    free(h_costs);
    free(open_map);
    free(closed_map);
}
