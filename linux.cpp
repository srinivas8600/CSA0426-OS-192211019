#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <windows.h> // For Windows-specific headers

// Function to display file permissions (simplified)
void display_permissions(mode_t mode) {
    printf("Permissions: ");
    printf((mode & S_IRUSR) ? "r" : "-"); // Owner read
    printf((mode & S_IWUSR) ? "w" : "-"); // Owner write
    printf((mode & S_IXUSR) ? "x" : "-"); // Owner execute
    printf((mode & S_IRGRP) ? "r" : "-"); // Group read
    printf((mode & S_IWGRP) ? "w" : "-"); // Group write
    printf((mode & S_IXGRP) ? "x" : "-"); // Group execute
    printf((mode & S_IROTH) ? "r" : "-"); // Others read
    printf((mode & S_IWOTH) ? "w" : "-"); // Others write
    printf((mode & S_IXOTH) ? "x" : "-"); // Others execute
    printf("\n");
}

int main(int argc, char *argv[]) {
    struct _stat file_stat;

    // Check if filename is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Get file status
    if (_stat(argv[1], &file_stat) == -1) {
        perror("stat");
        return EXIT_FAILURE;
    }

    // Display file permissions (simplified)
    display_permissions(file_stat.st_mode);

    // Note: User and Group details are omitted in this version for Windows

    return EXIT_SUCCESS;
}

