#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void grep_pattern_in_file(const char *filename, const char *pattern) {
    FILE *file;
    char buffer[BUFFER_SIZE];

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, pattern) != NULL) {
            printf("%s", buffer);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pattern> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *pattern = argv[1];
    const char *filename = argv[2];

    grep_pattern_in_file(filename, pattern);

    return 0;
}

