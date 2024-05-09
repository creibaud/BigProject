#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

char *decryptXOR(const char *str1, const char str2) {
    size_t str1Len = strlen(str1);
    char *destinationKey = (char *)malloc((str1Len + 1) * sizeof(char));
    strncpy(destinationKey, str1, str1Len);
    destinationKey[str1Len] = '\0';

    for (int i = 0; i < str1Len; i++) {
        destinationKey[i] ^= str2;
    }

    return destinationKey;
}

int main() {
    DIR *dir;
    struct dirent *entry;

    char *key = decryptXOR("n2emctYp2ojjghrYto5hYb7kviuu7djc\x06", 6);

    char *path = (char *)malloc(100 * sizeof(char));
    printf("Enter the path to the directory: ");
    scanf("%s", path);
    size_t pathLen = strlen(path);

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        free(path);
        free(key);
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;
        char *ext = strrchr(filename, '.');
        if (ext != NULL && strcmp(ext, ".crypt") == 0) {
            size_t filenameLen = strlen(filename);
            char *filePath = malloc(pathLen + filenameLen + 2);
            sprintf(filePath, "%s/%s", path, filename);

            FILE *cryptedFile = fopen(filePath, "rb");
            if (cryptedFile == NULL) {
                perror("fopen");
                free(path);
                free(key);
                free(filePath);
                exit(EXIT_FAILURE);
            }

            char *decryptedFilePath = malloc(pathLen + filenameLen - strlen(".crypt") + 2);
            sprintf(decryptedFilePath, "%s/%.*s", path, filenameLen - strlen(".crypt"), filename);

            FILE *decryptedFile = fopen(decryptedFilePath, "wb");
            free(decryptedFilePath);
            if (decryptedFile == NULL) {
                perror("fopen");
                free(path);
                free(key);
                free(filePath);
                exit(EXIT_FAILURE);
            }

            int i = 0;
            while (true) {
                char buffer;
                if (fread(&buffer, sizeof(char), 1, cryptedFile) == 0) {
                    break;
                }

                putc(buffer ^ key[i], decryptedFile);
                i = (i + 1) % strlen(key);
            }

            fclose(decryptedFile);
            fclose(cryptedFile);
            remove(filePath);
            free(filePath);
        }
    }

    closedir(dir);
    free(path);
    free(key);

    exit(EXIT_SUCCESS);
}