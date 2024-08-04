#include <stdio.h>
#include <stdlib.h>

#define NUM_FRAMES 3 // Number of page frames (memory slots)
#define NUM_PAGES 10 // Total number of pages in the reference string

// Function to simulate FIFO paging
void fifo_paging(int pages[], int num_pages, int num_frames) {
    int frames[num_frames];
    int page_faults = 0;
    int index = 0; // Points to the next frame to be replaced

    // Initialize frames to -1 (indicating empty)
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
    }

    printf("Reference String: ");
    for (int i = 0; i < num_pages; i++) {
        printf("%d ", pages[i]);
    }
    printf("\n");

    printf("Page Frames: ");
    for (int i = 0; i < num_frames; i++) {
        printf("Frame %d: ", i);
        if (frames[i] == -1) {
            printf("Empty\n");
        } else {
            printf("%d\n", frames[i]);
        }
    }

    // Simulate FIFO Paging
    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        int page_found = 0;

        // Check if page is already in one of the frames
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                page_found = 1;
                break;
            }
        }

        // If page is not found in frames, replace the oldest page
        if (!page_found) {
            frames[index] = page;
            index = (index + 1) % num_frames; // Move to the next frame
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
    }

    printf("Total page faults: %d\n", page_faults);
}

int main() {
    int pages[NUM_PAGES] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};

    fifo_paging(pages, NUM_PAGES, NUM_FRAMES);

    return 0;
}

