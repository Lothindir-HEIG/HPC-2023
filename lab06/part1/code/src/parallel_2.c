#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>

#define MAX_CHARACTERS 26
#define CHUNK_SIZE 1024

void count_occurrences(FILE *file, int *occurrences)
{
    char buffer[CHUNK_SIZE];
    // Determine the file size
    fseek(file, 0, SEEK_END);
    long int fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("File size: %ld\n", fileSize);

    omp_set_num_threads(2);
    int l_occurrences[MAX_CHARACTERS * 2] = {0};
    int sum = 0;

// Read the file in parallel
#pragma omp parallel shared(file, fileSize) private(buffer, l_occurrences) reduction(+ : occurrences[ : MAX_CHARACTERS * 2]) reduction(+ : sum)
    {
        memset(l_occurrences, 0, sizeof(int) * MAX_CHARACTERS * 2);
        int threadNum = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
        long int chunkSize = fileSize / numThreads;
        long int startPos = threadNum * chunkSize;
        long int endPos = (threadNum == numThreads - 1) ? fileSize : startPos + chunkSize;
        printf("Thread %d: startPos = %ld, endPos = %ld\n", threadNum, startPos, endPos);

        fseek(file, startPos, SEEK_SET);
        long int remainingBytes = endPos - startPos;
        size_t chunkBytes = (remainingBytes < CHUNK_SIZE) ? remainingBytes : CHUNK_SIZE;
        size_t bytesRead;
#pragma omp critical
        bytesRead = fread(buffer, sizeof(char), chunkBytes, file);
        printf("Thread %d: read %ld bytes, remaining %ld\n", threadNum, bytesRead, remainingBytes);
        while (remainingBytes > 0 && bytesRead > 0)
        {

            // Count the occurrences of characters in the chunk
            for (size_t i = 0; i < bytesRead; i++)
            {
                char ch = buffer[i];
                // printf("Thread %d: read character %c\n", threadNum, ch);
                if (ch >= 'A' && ch <= 'Z')
                {
                    l_occurrences[ch - 'A']++;
                }
                else if (ch >= 'a' && ch <= 'z')
                {
                    l_occurrences[MAX_CHARACTERS + ch - 'a']++; // Utilisation du décalage correct
                }
            }
            remainingBytes -= bytesRead;
            if (remainingBytes <= 0)
            {
                break;
            }
            chunkBytes = (remainingBytes < CHUNK_SIZE) ? remainingBytes : CHUNK_SIZE;
#pragma omp critical
            bytesRead = fread(buffer, sizeof(char), chunkBytes, file);
            if (bytesRead == 0 && ferror(file))
            {
                // Print error from ferror
                printf("Thread %d: error reading file\n", threadNum);
                perror("An error occurred while reading the file.\n");
            }
            if (bytesRead == 0 && feof(file))
            {
                // Print error from feof
                printf("Thread %d: end of file reached\n", threadNum);
                if (remainingBytes > 0)
                {
                    printf("Thread %d: %ld bytes remaining\n", threadNum, remainingBytes);
                    break;
                }
            }
            printf("Thread %d: read %ld bytes, remaining %ld\n", threadNum, bytesRead, remainingBytes);
        }
        printf("Thread %d: aggregating results...\n", threadNum);
        for (int i = 0; i < MAX_CHARACTERS * 2; i++)
        {
            occurrences[i] += l_occurrences[i];
            sum += l_occurrences[i];
        }
    }
    printf("Sum: %d\n", sum);
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
    printf("Counting occurrences in %s...\n", path);
    printf("Number of threads: %d\n", omp_get_max_threads());
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
