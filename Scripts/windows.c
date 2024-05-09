#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

void removeExtensionCrypt(char *path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;
        char *ext = strrchr(filename, '.');
        char *cmd = (char *)malloc(strlen("move \"") + 2 * strlen(path) + 2 * strlen("\\") + 2 * strlen(filename) + strlen("\" \"") - strlen(".crypt") + strlen("\"") + 1);
        if (ext != NULL && strcmp(ext, ".crypt") == 0) {
            sprintf(cmd, "move \"%s\\%s\" \"%s\\%.*s\"", path, filename, path, (int)(strlen(filename) - strlen(".crypt")), filename);
            system(cmd);
        }
        free(cmd);
    }

    closedir(dir);
}

int main() {
    char *path = (char *)malloc(100 * sizeof(char));
    printf("Entrez le chemin pour lister les fichiers : ");
    scanf("%s", path);

    removeExtensionCrypt(path);
    
    FILE *backup;

    backup = popen("dir /s /b Backup.exe", "r");
    if (backup == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), backup)) {
        line[strcspn(line, "\n")] = '\0';
    }

    pclose(backup);
    system(line);
    
    removeExtensionCrypt(path);

    free(path);
    
    return 0;
}