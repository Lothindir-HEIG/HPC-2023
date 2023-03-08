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

// Structure simple définissant une position dans une grille
typedef struct Position{
    int x;
    int y;
} Position;

// Strucutre permettant de créer une liste chainée pour la lecture
// d'une grille
typedef struct Grid_Component {
    int value;
    struct Grid_Component *left;
    struct Grid_Component* right;
    struct Grid_Component* up;
    struct Grid_Component* down;
} Grid_Component;

// Strucutre représentant un noeud de l'alfabet A*
typedef struct Node
{
    Position *pos;
    int g;
    int h;
    int f;
    struct Node *parent;
} Node;

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

//Cette fonction initialise un tableau en 2 dimension en lisant un fichier. Elle permet
//  également de trouver le point de départ (A) et le point d'arriver (D) si spécifier dans 
//  le fichier input.txt
int** init_grid_tab(int rows, int columns, int DEBUG, Position* src, Position* dst);

//Cette fonction initialise une liste chainée en lisant un fichier. Elle permet
//  également de trouver le point de départ (A) et le point d'arriver (D) si spécifier dans 
//  le fichier input.txt
Grid_Component* init_grid_struct(int rows, int cols, int DEBUG, Position* src, Position* dst);
