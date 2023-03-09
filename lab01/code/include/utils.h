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

// Couleurs pour l'affichage
#define ANSI_COLOR_RST "\x1B[0m"
#define ANSI_COLOR_RED "\x1B[31m"
#define ANSI_COLOR_GRN "\x1B[32m"
#define ANSI_COLOR_YEL "\x1B[33m"
#define ANSI_COLOR_BLU "\x1B[34m"
#define ANSI_COLOR_CYN "\x1B[36m"

// Enum à utiliser pour parcourir les voisins d'un noeud
#define GRID_DIRECTION_RIGHT 0
#define GRID_DIRECTION_LEFT 1
#define GRID_DIRECTION_UP 2
#define GRID_DIRECTION_DOWN 3

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
    int h;
    int f;
} Costs;

typedef struct Grid_Node
{
    Grid_Component *component;
    Position pos;
    Costs costs;
    struct Grid_Node *parent;
} Grid_Node;

// Strucutre représentant un noeud de l'alfabet A*
typedef struct Node
{
    Position pos;
    Costs costs;
    struct Node *parent;
} Node;

// Strucutre permettant de créer une liste chainée de Nodes
typedef struct LinkedNode
{
    Grid_Node *node;
    struct LinkedNode *next;
} LinkedNode;

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

// Cette fonction permet d'afficher une grille en 2 dimension avec les differentes listes (open, closed, path)
void print_grid(Grid *grid, Node **open_list, int open_list_size, Node **closed_list, int closed_list_size, Node **path, int path_size);

Node *is_in_list(Node **list, int list_size, int x, int y);
