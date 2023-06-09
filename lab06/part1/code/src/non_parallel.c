#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CHARACTERS 26

void count_occurrences(FILE *file, int *occurrences)
{
    int ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            occurrences[ch - 'A']++;
        }
        else if (ch >= 'a' && ch <= 'z')
        {
            occurrences[MAX_CHARACTERS + ch - 'a']++; // Utilisation du décalage correct
        }
    }
}

int main(int argc, char **argv)
{
    char *path;
    if (argc < 2)
    {
        path = "../data/big.txt";
    }
    else
    {
        path = argv[1];
    }

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    int occurrences[MAX_CHARACTERS * 2] = {0}; // Augmentation de la taille du tableau

    clock_t start = clock();

    count_occurrences(file, occurrences);

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

    fclose(file);

    // Affichage des occurrences
    for (int i = 0; i < MAX_CHARACTERS; i++)
    {
        char uppercase = 'A' + i;
        char lowercase = 'a' + i;
        printf("%c: %d\n", uppercase, occurrences[i]);
        printf("%c: %d\n", lowercase, occurrences[MAX_CHARACTERS + i]); // Utilisation du décalage
    }

    printf("Temps de traitement : %.2f secondes\n", elapsed_time);

    return 0;
}
