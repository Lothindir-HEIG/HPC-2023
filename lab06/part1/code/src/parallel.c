#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

#define MAX_CHARACTERS 26
#define BUFFER_SIZE 10000

void count_occurrences(FILE *file, int *occurrences, size_t buffer_size)
{
    char *buffer = malloc(buffer_size * sizeof(char));
    // Determine the file size
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    size_t remainingBytes = fileSize;
    size_t chunkBytes = (remainingBytes < buffer_size) ? remainingBytes : buffer_size;
    size_t bytesRead;
    int l_occurrences[MAX_CHARACTERS * 2] = {0}; // Augmentation de la taille du tableau
    while (remainingBytes > 0 && (bytesRead = fread(buffer, sizeof(char), chunkBytes, file)) > 0)
    {
// Count the occurrences of characters in the chunk
#pragma omp parallel shared(buffer, bytesRead) firstprivate(l_occurrences) reduction(+ : occurrences[ : MAX_CHARACTERS * 2])
        {
#pragma omp for
            for (size_t i = 0; i < bytesRead; i++)
            {
                char ch = buffer[i];
                if (ch >= 'A' && ch <= 'Z')
                {
                    l_occurrences[ch - 'A']++;
                }
                else if (ch >= 'a' && ch <= 'z')
                {
                    l_occurrences[MAX_CHARACTERS + ch - 'a']++; // Utilisation du décalage correct
                }
            }

            for (int i = 0; i < MAX_CHARACTERS * 2; i++)
            {
                occurrences[i] += l_occurrences[i];
            }
        }

        remainingBytes -= bytesRead;
        if (remainingBytes <= 0)
        {
            break;
        }
        chunkBytes = (remainingBytes < buffer_size) ? remainingBytes : buffer_size;
    }
    free(buffer);
}

int main(int argc, char **argv)
{
    char *path;
    size_t buffer_size;
    if (argc < 2)
    {
        path = "../data/big.txt";
        buffer_size = 65536;
    }
    else if (argc < 3)
    {
        path = argv[1];
        buffer_size = 65536;
    }
    else
    {
        path = argv[1];
        buffer_size = atoi(argv[2]);
    }

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    int occurrences[MAX_CHARACTERS * 2] = {0}; // Augmentation de la taille du tableau

    count_occurrences(file, occurrences, buffer_size);

    fclose(file);

    // Affichage des occurrences
    for (int i = 0; i < MAX_CHARACTERS; i++)
    {
        char uppercase = 'A' + i;
        char lowercase = 'a' + i;
        printf("%c: %d\n", uppercase, occurrences[i]);
        printf("%c: %d\n", lowercase, occurrences[MAX_CHARACTERS + i]); // Utilisation du décalage
    }

    return 0;
}
