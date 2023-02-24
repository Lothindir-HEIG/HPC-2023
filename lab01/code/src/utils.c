/* Bruno Da Rocha Carvalho / REDS / HEIG-VD
 * HPC - 24.02.23
 * utils.c
 * 
 * Utilitaire pour la création de grille.
 */

#include "utils.h"


int** init_grid_tab(int rows, int columns, int DEBUG, Position* src, Position* dst){
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }
    int nb_rows = rows, nb_columns = columns;
    int **grid = malloc(nb_rows * sizeof(int *));
    for (int i = 0; i < nb_rows; i++) {
        grid[i] = malloc(nb_columns * sizeof(int));
    }

    for (int i = 0; i < nb_rows; i++) {
        for (int j = 0; j < nb_columns; j++) {
            char c;
            int val;
            fscanf(file, " %c", &c);

            if (c == 'A'){
                src->x = j;
                src->y = i;
                val = 0;
            } else if (c == 'B'){
                dst->x = j;
                dst->y = i;
                val = 0;
            } else {
                 val = (int)c -48;
            }

            grid[i][j] = val;
        }
    }

    if (DEBUG){
        printf("Contenu du tableau :\n");
        for (int i = 0; i < nb_rows; i++) {
            for (int j = 0; j < nb_columns; j++) {
                printf("%d ", grid[i][j]);
            }
            printf("\n");
        }
    }

    fclose(file);

    return grid;
}

Grid_Component* init_grid_struct(int rows, int cols, int DEBUG, Position* src, Position* dst) {
    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open file %s", "input.txt");
        exit(1);
    }

    Grid_Component* head = NULL;
    Grid_Component* curr = NULL;
    Grid_Component* prev_row[cols];

    for (int i = 0; i < cols; i++) {
        prev_row[i] = NULL;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char c;
            int val;
            fscanf(fp, " %c", &c);
            if (c == 'A'){
                src->x = j;
                src->y = i;
                val = 0;
            } else if (c == 'B'){
                dst->x = j;
                dst->y = i;
                val = 0;
            } else {
                 val = (int)c -48;
            }
            Grid_Component* node = malloc(sizeof(Grid_Component));
            node->value = val;
            node->left = NULL;
            node->right = NULL;
            node->up = NULL;
            node->down = NULL;

            if (j == 0) {
                if (head == NULL) {
                    head = node;
                }
                curr = node;
            } else {
                curr->right = node;
                node->left = curr;
                curr = curr->right;
            }

            if (prev_row[j] != NULL) {
                prev_row[j]->down = node;
                node->up = prev_row[j];
            }

            prev_row[j] = node;
        }
    }

    if (DEBUG){
        Grid_Component* curr_row = head;

        for (int i = 0; i < rows; i++) {
            Grid_Component* curr = curr_row;
            for (int j = 0; j < cols; j++) {
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

