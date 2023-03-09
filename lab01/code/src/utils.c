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

void print_grid(Grid *grid, Node **open_list, int open_list_size, Node **closed_list, int closed_list_size, Node **path, int path_size)
{
    printf("Contenu de la grille :\n");

    printf("╭─");
    for (int c = 0; c < grid->cols; c++)
        printf("──");
    printf("╮\n");

    for (int i = 0; i < grid->rows; i++)
    {
        printf("│ ");
        for (int j = 0; j < grid->cols; j++)
        {
            if (grid->src->x == j && grid->src->y == i)
                printf("%sS%s ", ANSI_COLOR_GRN, ANSI_COLOR_RST);
            else if (grid->dst->x == j && grid->dst->y == i)
                printf("%sD%s ", ANSI_COLOR_RED, ANSI_COLOR_RST);
            else if (is_in_list(path, path_size, j, i))
                printf("%s●%s ", ANSI_COLOR_YEL, ANSI_COLOR_RST);
            else if (grid->data.grid[i][j] == 1)
                printf("■ ");
            else if (is_in_list(open_list, open_list_size, j, i))
                printf("%s⬡%s ", ANSI_COLOR_CYN, ANSI_COLOR_RST);
            else if (is_in_list(closed_list, closed_list_size, j, i))
                printf("%s⬢%s ", ANSI_COLOR_BLU, ANSI_COLOR_RST);
            else
                printf("- ");
        }
        printf("│\n");
    }
    printf("╰─");
    for (int c = 0; c < grid->cols; c++)
        printf("──");
    printf("╯\n");
}

Node *is_in_list(Node **list, int list_size, int x, int y)
{
    for (int i = 0; i < list_size; i++)
    {
        if (list[i]->pos.x == x && list[i]->pos.y == y)
        {
            return list[i];
        }
    }
    return 0;
}
