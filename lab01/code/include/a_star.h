#include "utils.h"

// Enum à utiliser pour les valeurs de retour des fonction compute_path
typedef enum
{
    PATH_FOUND = 0,
    ERROR = -1,
    NO_PATH_FOUND = 1
} Path_error;

// Cette fonction permet de calculer le chemin le plus cours entre src et dst en évitant les obstacles
//   présent sur le chemin.
// Path_error compute_path_struct(Position* src, Position* dst, Grid_Component* head, void* path);
Path_error compute_path_struct(Grid *grid, int DEBUG, void *path);

// Cette fonction permet de calculer le chemin le plus cours entre src et dst en évitant les obstacles
//   présent sur le chemin.
// Path_error compute_path_tab(Position* src, Position* dst, int** grid, void* path);
Path_error compute_path_tab(Grid *grid, int DEBUG, void *path);