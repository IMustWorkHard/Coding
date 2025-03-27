#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Structure to store data for each thread
typedef struct {
    long iterations; // Total number of iterations for the series
    long thread_id;  // Unique ID of the thread
    double result;   // Partial sum calculated by the thread
} ThreadData;

/**
 * Function executed by each thread to compute part of the Leibniz series for π.
 *
 * @param arg Pointer to a ThreadData structure containing thread-specific parameters.
 * @return NULL (result is stored in the ThreadData structure).
 */
void *calculate_pi(void *arg) {
    ThreadData *data = (ThreadData *)arg; // Cast void pointer to ThreadData pointer
    long iterations = data->iterations;   // Total number of iterations
    long thread_id = data->thread_id;     // Thread ID used for assigning terms
    
    double sum = 0.0;

    // Leibniz series: π ≈ 4 * (1 - 1/3 + 1/5 - 1/7 + 1/9 - ...)
    // Each thread calculates a subset of terms based on its thread_id
    for (long i = thread_id; i < iterations; i += 4) { 
        sum += (i % 2 == 0 ? 1.0 : -1.0) / (2.0 * i + 1.0);
    }
    
    data->result = sum; // Store partial sum in the thread data structure
    return NULL;
}

int main() {
    long iterations; // Total iterations for the series
    int num_threads; // Number of threads to use

    // Get user input for the number of iterations and threads
    printf("Enter the number of iterations: ");
    scanf("%ld", &iterations);
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    // Allocate memory for thread handles and thread data
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data = (ThreadData *)malloc(num_threads * sizeof(ThreadData));
    
    if (threads == NULL || thread_data == NULL) {
        printf("Memory allocation failed\n");
        return 1; // Exit if memory allocation fails
    }

    // Create threads to compute parts of the Leibniz series
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].iterations = iterations;
        thread_data[i].thread_id = i;  // Assign unique thread ID
        thread_data[i].result = 0.0;   // Initialize partial sum
        pthread_create(&threads[i], NULL, calculate_pi, (void *)&thread_data[i]);
    }

    double total_sum = 0.0;

    // Wait for all threads to complete and sum their results
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].result;
    }

    // Compute final approximation of Pi using the Leibniz formula (π ≈ 4 * sum)
    double pi = total_sum * 4.0;
    printf("Calculated value of Pi: %.15f\n", pi);

    // Free allocated memory
    free(threads);
    free(thread_data);
    
    return 0;
}
