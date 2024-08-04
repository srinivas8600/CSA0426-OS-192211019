#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

void list_directory(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    printf("Contents of directory '%s':\n", dir_path);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *dir_path = argv[1];
    list_directory(dir_path);

    return 0;
}

