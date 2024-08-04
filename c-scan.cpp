#include <stdio.h>
#include <stdlib.h>

#define MAX_REQUESTS 10  // Maximum number of disk requests

// Function to compare two integers (used by qsort)
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Function to perform C-SCAN disk scheduling
void cscan_disk_scheduling(int requests[], int num_requests, int start_position, int disk_size) {
    // Sort requests in ascending order
    qsort(requests, num_requests, sizeof(int), compare);

    int total_seek_time = 0;
    int current_position = start_position;
    int i;

    printf("Disk scheduling sequence:\n");

    // Handle requests to the right of the start position
    for (i = 0; i < num_requests; i++) {
        if (requests[i] >= current_position) {
            int seek_time = abs(requests[i] - current_position);
            printf("Move from %d to %d (Seek time: %d)\n", current_position, requests[i], seek_time);
            total_seek_time += seek_time;
            current_position = requests[i];
        }
    }

    // Move to the beginning of the disk if necessary
    if (current_position < disk_size) {
        int seek_time = abs(disk_size - current_position) + abs(0 - requests[0]);
        printf("Move from %d to %d (Seek time: %d)\n", current_position, disk_size, abs(disk_size - current_position));
        printf("Move from %d to %d (Seek time: %d)\n", 0, requests[0], abs(0 - requests[0]));
        total_seek_time += seek_time;
        current_position = requests[0];
    }

    // Handle requests that were to the left of the start position
    for (i = 0; i < num_requests; i++) {
        if (requests[i] < start_position) {
            int seek_time = abs(requests[i] - current_position);
            printf("Move from %d to %d (Seek time: %d)\n", current_position, requests[i], seek_time);
            total_seek_time += seek_time;
            current_position = requests[i];
        }
    }

    printf("Total seek time: %d\n", total_seek_time);
}

int main() {
    int requests[MAX_REQUESTS];
    int num_requests;
    int start_position;
    int disk_size;

    // Input the number of disk requests
    printf("Enter the number of disk requests (max %d): ", MAX_REQUESTS);
    scanf("%d", &num_requests);

    if (num_requests > MAX_REQUESTS) {
        printf("Exceeds maximum number of requests. Truncating to %d.\n", MAX_REQUESTS);
        num_requests = MAX_REQUESTS;
    }

    // Input the disk requests
    printf("Enter the disk requests:\n");
    for (int i = 0; i < num_requests; i++) {
        printf("Request %d: ", i + 1);
        scanf("%d", &requests[i]);
    }

    // Input the starting position of the disk arm
    printf("Enter the starting position of the disk arm: ");
    scanf("%d", &start_position);

    // Input the disk size (total number of cylinders)
    printf("Enter the disk size (number of cylinders): ");
    scanf("%d", &disk_size);

    // Perform C-SCAN disk scheduling
    cscan_disk_scheduling(requests, num_requests, start_position, disk_size);

    return 0;
}

