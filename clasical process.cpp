#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // For sleep function

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

// Shared buffer and related variables
int buffer[BUFFER_SIZE];
int in = 0;  // Index where the producer inserts data
int out = 0; // Index where the consumer retrieves data

sem_t empty; // Semaphore to count empty slots
sem_t full;  // Semaphore to count full slots
pthread_mutex_t mutex; // Mutex for mutual exclusion

// Producer function
void* producer(void* arg) {
    int item;
    while (1) {
        // Produce an item
        item = rand() % 100;
        sem_wait(&empty); // Wait for an empty slot
        pthread_mutex_lock(&mutex); // Lock the mutex
        
        // Add item to buffer
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        printf("Producer %d produced %d\n", *((int*)arg), item);
        
        pthread_mutex_unlock(&mutex); // Unlock the mutex
        sem_post(&full); // Signal that there is a full slot

        sleep((rand() % 5) + 1); // Simulate variable production time between 1 and 5 seconds
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    int item;
    while (1) {
        sem_wait(&full); // Wait for a full slot
        pthread_mutex_lock(&mutex); // Lock the mutex
        
        // Remove item from buffer
        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumer %d consumed %d\n", *((int*)arg), item);
        
        pthread_mutex_unlock(&mutex); // Unlock the mutex
        sem_post(&empty); // Signal that there is an empty slot

        sleep((rand() % 5) + 1); // Simulate variable consumption time between 1 and 5 seconds
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE); // All slots are initially empty
    sem_init(&full, 0, 0); // No slots are initially full
    pthread_mutex_init(&mutex, NULL);

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, (void*)&producer_ids[i]);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, (void*)&consumer_ids[i]);
    }

    // Wait for threads to finish (they actually run indefinitely in this example)
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Clean up
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

