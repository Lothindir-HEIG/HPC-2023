/* Bruno Da Rocha Carvalho/ REDS / HEIG-VD
 * HPC - 24.02.23
 *
 * L'utilisateur doit choisir entre deux génération de grille pour
 * l'utilisation de A*.
 *
 * ./a_star tab 20 11
 * ./a_star struct 20 11
 */

#include "a_star.h"

#define PARAM_COUNT 4
#define PARAM_A_STAR_TAB "tab"
#define PARAM_A_STAR_STRUCT "struct"

#define DEBUG 1

int main(int argc, char **argv)
{
    printf("Labo1 HPC - A* implementation\n");
    uint8_t type_a_star;
    int rows_map, cols_map;
    int **grid;
    Grid_Component *head;
    Position *src = malloc(sizeof(Position));
    Position *dst = malloc(sizeof(Position));
    Path_error error;

    if (argc != PARAM_COUNT)
    {
        fprintf(stderr, "usage: %s list|array data_len\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!strcmp(argv[1], PARAM_A_STAR_TAB))
        type_a_star = 0;

    else if (!strcmp(argv[1], PARAM_A_STAR_STRUCT))
        type_a_star = 1;

    else
    {
        fprintf(stderr, "wrong parameter \"%s\"\n", argv[1]);
        return EXIT_FAILURE;
    }

    cols_map = atol(argv[2]);
    rows_map = atol(argv[3]);

    Grid *grid_struct = malloc(sizeof(Grid));
    grid_struct->rows = rows_map;
    grid_struct->cols = cols_map;
    grid_struct->src = src;
    grid_struct->dst = dst;

    if (type_a_star)
    {
        printf("Generating struct\n");
        head = init_grid_struct(rows_map, cols_map, DEBUG, src, dst);

        printf("Source point : (%d, %d)\n", src->x, src->y);
        printf("Destination point : (%d, %d)\n", dst->x, dst->y);

        grid_struct->data.head = head;
        error = compute_path_struct(grid_struct, NULL);

        // Free de la liste chainée
        Grid_Component *curr = head;
        while (curr != NULL)
        {
            Grid_Component *next = curr->right;
            while (curr != NULL)
            {
                Grid_Component *temp = curr;
                curr = curr->up;
                free(temp);
            }
            curr = next;
        }
    }
    else
    {
        printf("Generating grid\n");
        grid = init_grid_tab(rows_map, cols_map, DEBUG, src, dst);

        printf("Source point : (%d, %d)\n", src->x, src->y);
        printf("Destination point : (%d, %d)\n", dst->x, dst->y);

        grid_struct->data.grid = grid;
        error = compute_path_tab(grid_struct, NULL);

        for (int i = 0; i < rows_map; i++)
        {
            free(grid[i]);
        }
        free(grid);
    }

    free(src);
    free(dst);
    free(grid_struct);

    switch (error)
    {
    case NO_PATH_FOUND:
        printf("No path were found\n");
        break;
    case PATH_FOUND:
        printf("The path is found\n");
        break;
    case ERROR:
        printf("An error occured during the path computing\n");
        break;
    }

    return EXIT_SUCCESS;
}