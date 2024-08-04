#include <stdio.h>
#include <stdlib.h>

#define NUM_FRAMES 3 // Number of page frames (memory slots)
#define NUM_PAGES 10 // Total number of pages in the reference string

// Function to simulate LRU paging
void lru_paging(int pages[], int num_pages, int num_frames) {
    int frames[num_frames];
    int last_used[num_frames];
    int page_faults = 0;
    int time = 0; // Simulation of time to track last used

    // Initialize frames and last_used arrays
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    printf("Reference String: ");
    for (int i = 0; i < num_pages; i++) {
        printf("%d ", pages[i]);
    }
    printf("\n");

    // Simulate LRU Paging
    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        int page_found = 0;
        int lru_index = 0;
        int min_time = time;

        // Check if page is already in one of the frames
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                page_found = 1;
                last_used[j] = time; // Update last used time
                break;
            }
        }

        // If page is not found in frames, replace the LRU page
        if (!page_found) {
            // Find the index of the LRU page
            for (int j = 0; j < num_frames; j++) {
                if (last_used[j] < min_time) {
                    min_time = last_used[j];
                    lru_index = j;
                }
            }
            // Replace the LRU page
            frames[lru_index] = page;
            last_used[lru_index] = time;
            page_faults++;
        }

        // Print the current state of frames
        printf("After accessing page %d: ", page);
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == -1) {
                printf("Empty ");
            } else {
                printf("%d ", frames[j]);
            }
        }
        printf("\n");

        time++; // Increment time after each page access
    }

    printf("Total page faults: %d\n", page_faults);
}

int main() {
    int pages[NUM_PAGES] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};

    lru_paging(pages, NUM_PAGES, NUM_FRAMES);

    return 0;
}

