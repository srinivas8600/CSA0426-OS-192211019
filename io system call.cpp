#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

void print_file_status(const char *filename) {
    struct stat file_stat;

    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("File: %s\n", filename);
    printf("  Size: %ld bytes\n", file_stat.st_size);
    printf("  Mode: %o (octal)\n", file_stat.st_mode & ~S_IFMT);
}

void read_file_with_seek(const char *filename) {
    int fd;
    char buffer[128];
    ssize_t bytes_read;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Move to the 10th byte in the file
    if (lseek(fd, 10, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }

    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_read] = '\0'; // Null-terminate the string
    printf("Read from file after seeking:\n%s\n", buffer);

    close(fd);
}

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
        printf("  %s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <file> <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *file = argv[1];
    const char *directory = argv[2];

    // Print file status
    print_file_status(file);

    // Read from the file after seeking
    read_file_with_seek(file);

    // List directory contents
    list_directory(directory);

    return 0;
}

