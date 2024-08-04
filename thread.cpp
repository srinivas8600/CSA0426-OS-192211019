#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Function that will be executed by the threads
void* thread_function(void* arg) {
    pthread_t self = pthread_self();
    printf("Thread ID: %lu is running\n", (unsigned long)self);

    sleep(2); // Simulate work by sleeping

    printf("Thread ID: %lu is exiting\n", (unsigned long)self);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[2];
    pthread_t main_thread = pthread_self();

    // Create threads
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        printf("Created thread %d with ID: %lu\n", i + 1, (unsigned long)threads[i]);
    }

    // Check if threads are equal to main thread (though this is more illustrative)
    for (int i = 0; i < 2; i++) {
        if (pthread_equal(threads[i], main_thread)) {
            printf("Thread ID: %lu is equal to main thread ID: %lu\n", (unsigned long)threads[i], (unsigned long)main_thread);
        } else {
            printf("Thread ID: %lu is NOT equal to main thread ID: %lu\n", (unsigned long)threads[i], (unsigned long)main_thread);
        }
    }

    // Join threads
    for (int i = 0; i < 2; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
        printf("Joined thread %d with ID: %lu\n", i + 1, (unsigned long)threads[i]);
    }

    printf("Main thread ID: %lu is exiting\n", (unsigned long)main_thread);
    return 0;
}

