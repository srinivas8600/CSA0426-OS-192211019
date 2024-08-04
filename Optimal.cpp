#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_FRAMES 3 // Number of page frames (memory slots)
#define NUM_PAGES 10 // Total number of pages in the reference string

// Function to find the index of the page to be replaced
int find_optimal_page(int frames[], int page_index[], int num_frames, int current_index, int pages[], int num_pages) {
    int max_distance = -1;
    int page_to_replace = 0;

    for (int i = 0; i < num_frames; i++) {
        int j;
        for (j = current_index; j < num_pages; j++) {
            if (frames[i] == pages[j]) {
                page_index[i] = j;
                break;
            }
        }
        // If page not found in future references, it will be replaced
        if (j == num_pages) {
            return i;
        }
        if (page_index[i] > max_distance) {
            max_distance = page_index[i];
            page_to_replace = i;
        }
    }
    return page_to_replace;
}

// Function to simulate Optimal paging
void optimal_paging(int pages[], int num_pages, int num_frames) {
    int frames[num_frames];
    int page_index[num_frames];
    int page_faults = 0;

    // Initialize frames and page_index arrays
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        page_index[i] = -1;
    }

    printf("Reference String: ");
    for (int i = 0; i < num_pages; i++) {
        printf("%d ", pages[i]);
    }
    printf("\n");

    // Simulate Optimal Paging
    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        int page_found = 0;

        // Check if the page is already in one of the frames
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                page_found = 1;
                break;
            }
        }

        // If page is not found in frames, replace the optimal page
        if (!page_found) {
            int replace_index = find_optimal_page(frames, page_index, num_frames, i, pages, num_pages);
            frames[replace_index] = page;
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

    optimal_paging(pages, NUM_PAGES, NUM_FRAMES);

    return 0;
}

