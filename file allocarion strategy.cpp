#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100 // Maximum number of records in the file

typedef struct {
    char data[256]; // Size of each record
} Record;

// Function to initialize the file with records
void initialize_file(Record file[], int *count) {
    printf("Enter the number of records to initialize (max %d): ", MAX_RECORDS);
    scanf("%d", count);
    getchar(); // Consume newline character left by scanf

    if (*count > MAX_RECORDS) {
        printf("Exceeds maximum record limit of %d. Truncating to %d.\n", MAX_RECORDS, MAX_RECORDS);
        *count = MAX_RECORDS;
    }

    for (int i = 0; i < *count; i++) {
        printf("Enter data for record %d: ", i + 1);
        fgets(file[i].data, sizeof(file[i].data), stdin);
        file[i].data[strcspn(file[i].data, "\n")] = '\0'; // Remove trailing newline
    }
}

// Function to read records sequentially
void read_records(Record file[], int count) {
    printf("Reading all records sequentially:\n");
    for (int i = 0; i < count; i++) {
        printf("Record %d: %s\n", i + 1, file[i].data);
    }
}

int main() {
    Record file[MAX_RECORDS];
    int record_count = 0;

    // Initialize the file with records
    initialize_file(file, &record_count);

    // Read all records sequentially
    read_records(file, record_count);

    return 0;
}

