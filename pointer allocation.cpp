#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 256  // Size of each block

// Structure for a file block
typedef struct Block {
    char data[BLOCK_SIZE]; // Data stored in each block
    struct Block *next;    // Pointer to the next block
} Block;

// Function to create a new block
Block* create_block(const char *data) {
    Block *new_block = (Block *)malloc(sizeof(Block));
    if (new_block == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strncpy(new_block->data, data, BLOCK_SIZE - 1);
    new_block->data[BLOCK_SIZE - 1] = '\0'; // Ensure null-termination
    new_block->next = NULL;
    return new_block;
}

// Function to initialize the file with blocks
void initialize_file(Block **head, Block **tail, int num_blocks) {
    Block *current = NULL;
    for (int i = 0; i < num_blocks; i++) {
        char buffer[BLOCK_SIZE];
        printf("Enter data for block %d: ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline

        Block *new_block = create_block(buffer);

        if (*head == NULL) {
            *head = new_block;
            *tail = new_block;
        } else {
            (*tail)->next = new_block;
            *tail = new_block;
        }
    }
}

// Function to read the file blocks
void read_file(Block *head) {
    Block *current = head;
    printf("Reading file blocks:\n");
    while (current != NULL) {
        printf("Block: %s\n", current->data);
        current = current->next;
    }
}

int main() {
    Block *head = NULL; // Pointer to the first block
    Block *tail = NULL; // Pointer to the last block

    int num_blocks;
    // Initialize the file with blocks
    printf("Enter the number of blocks: ");
    scanf("%d", &num_blocks);
    getchar(); // Consume newline character left by scanf

    initialize_file(&head, &tail, num_blocks);

    // Read the file blocks
    read_file(head);

    // Free allocated memory
    Block *current = head;
    while (current != NULL) {
        Block *next = current->next;
        free(current);
        current = next;
    }

    return 0;
}

