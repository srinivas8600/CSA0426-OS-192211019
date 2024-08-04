#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 10   // Maximum number of blocks in the file
#define BLOCK_SIZE 256  // Size of each block

// Structure for a file block
typedef struct {
    char data[BLOCK_SIZE]; // Data stored in each block
} FileBlock;

// Function to initialize the file with blocks
void initialize_file(FileBlock *file_blocks[], int num_blocks) {
    for (int i = 0; i < num_blocks; i++) {
        file_blocks[i] = (FileBlock *)malloc(sizeof(FileBlock));
        if (file_blocks[i] == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        printf("Enter data for block %d: ", i + 1);
        fgets(file_blocks[i]->data, sizeof(file_blocks[i]->data), stdin);
        file_blocks[i]->data[strcspn(file_blocks[i]->data, "\n")] = '\0'; // Remove trailing newline
    }
}

// Function to read the file blocks using the index block
void read_file(FileBlock *file_blocks[], int num_blocks) {
    printf("Reading file blocks:\n");
    for (int i = 0; i < num_blocks; i++) {
        printf("Block %d: %s\n", i + 1, file_blocks[i]->data);
    }
}

int main() {
    FileBlock *file_blocks[MAX_BLOCKS]; // Array of pointers to file blocks
    int num_blocks;

    // Initialize the file with blocks
    printf("Enter the number of blocks (max %d): ", MAX_BLOCKS);
    scanf("%d", &num_blocks);
    getchar(); // Consume newline character left by scanf

    if (num_blocks > MAX_BLOCKS) {
        printf("Exceeds maximum block limit of %d. Truncating to %d.\n", MAX_BLOCKS, MAX_BLOCKS);
        num_blocks = MAX_BLOCKS;
    }

    initialize_file(file_blocks, num_blocks);

    // Read the file blocks
    read_file(file_blocks, num_blocks);

    // Free allocated memory
    for (int i = 0; i < num_blocks; i++) {
        free(file_blocks[i]);
    }

    return 0;
}
s

