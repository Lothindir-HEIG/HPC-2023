/* Bruno Da Rocha Carvalho / REDS / HEIG-VD
 * HPC - 24.02.23
 * utils.h
 *
 * Utilitaire pour la création de grille.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "min_heap.h"

// Couleurs pour l'affichage
#define ANSI_COLOR_RST "\x1B[0m"
#define ANSI_COLOR_RED "\x1B[31m"
#define ANSI_COLOR_GRN "\x1B[32m"
#define ANSI_COLOR_YEL "\x1B[33m"
#define ANSI_COLOR_BLU "\x1B[34m"
#define ANSI_COLOR_MAG "\x1B[35m"
#define ANSI_COLOR_CYN "\x1B[36m"

// Structure simple définissant une position dans une grille
typedef struct Position
{
    int x;
    int y;
} Position;

// Strucutre permettant de créer une liste chainée pour la lecture
// d'une grille
typedef struct Grid_Component
{
    int value;
    struct Grid_Component *left;
    struct Grid_Component *right;
    struct Grid_Component *up;
    struct Grid_Component *down;
} Grid_Component;

typedef struct Costs
{
    int g;
    int f;
} Costs;

// Structure permettant de créer une grille contenant les bornes et la structure de données (tableau ou liste chainée)
typedef struct Grid
{
    int rows;
    int cols;
    Position *src;
    Position *dst;
    union data
    {
        int **grid;
        Grid_Component *head;
    } data;

} Grid;

// Cette fonction initialise un tableau en 2 dimension en lisant un fichier. Elle permet
//   également de trouver le point de départ (A) et le point d'arriver (D) si spécifier dans
//   le fichier input.txt
int **init_grid_tab(int rows, int columns, int DEBUG, Position *src, Position *dst, char *filename);

// Cette fonction initialise une liste chainée en lisant un fichier. Elle permet
//   également de trouver le point de départ (A) et le point d'arriver (D) si spécifier dans
//   le fichier input.txt
Grid_Component *init_grid_struct(int rows, int cols, int DEBUG, Position *src, Position *dst, char *filename);

void print_grid(Grid *grid, Position *path, bool **open_map, bool **closed_list_map, bool is_struct);

void free_lists(Grid *grid, MinHeap *open_set, Position **parent_set, int **h_costs, bool **open_map, bool **closed_map);
