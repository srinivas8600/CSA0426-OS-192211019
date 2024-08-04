#include <stdio.h>
#include <stdlib.h> // Include this header for abs function

#define NUM_REQUESTS 10 // Maximum number of disk requests

// Function to calculate the total seek time and print the sequence of requests
void fcfs_disk_scheduling(int requests[], int num_requests, int start_position) {
    int total_seek_time = 0;
    int current_position = start_position;

    printf("Disk scheduling sequence:\n");

    // Process each request in the order they arrive
    for (int i = 0; i < num_requests; i++) {
        int request_position = requests[i];
        int seek_time = abs(request_position - current_position);

        printf("Move from %d to %d (Seek time: %d)\n", current_position, request_position, seek_time);

        total_seek_time += seek_time;
        current_position = request_position;
    }

    printf("Total seek time: %d\n", total_seek_time);
}

int main() {
    int requests[NUM_REQUESTS];
    int num_requests;
    int start_position;

    // Input the number of disk requests
    printf("Enter the number of disk requests (max %d): ", NUM_REQUESTS);
    scanf("%d", &num_requests);

    if (num_requests > NUM_REQUESTS) {
        printf("Exceeds maximum number of requests. Truncating to %d.\n", NUM_REQUESTS);
        num_requests = NUM_REQUESTS;
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

    // Perform FCFS disk scheduling
    fcfs_disk_scheduling(requests, num_requests, start_position);

    return 0;
}

