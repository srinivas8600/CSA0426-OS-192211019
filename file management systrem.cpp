#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void create_file(const char *filename) {
    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("File '%s' created successfully.\n", filename);
    close(fd);
}

void write_file(const char *filename, const char *data) {
    int fd = open(filename, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(fd, data, strlen(data));
    if (bytes_written == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Wrote %zd bytes to '%s'.\n", bytes_written, filename);
    close(fd);
}

void read_file(const char *filename) {
    char buffer[128];
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_read] = '\0'; // Null-terminate the buffer
    printf("Contents of '%s':\n%s\n", filename, buffer);

    close(fd);
}

void delete_file(const char *filename) {
    if (unlink(filename) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
    printf("File '%s' deleted successfully.\n", filename);
}

int main() {
    const char *filename = "example_file.txt";
    const char *data = "This is an example of file management in C.";

    // Create a file
    create_file(filename);

    // Write data to the file
    write_file(filename, data);

    // Read data from the file
    read_file(filename);

    // Delete the file
    delete_file(filename);

    return 0;
}

