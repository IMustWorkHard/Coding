#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

/**
 * Structure to hold thread-specific data.
 * Each thread processes a file, counts prime numbers, and stores them.
 */
typedef struct {
    const char *filename;  // Input file name
    int prime_count;       // Number of prime numbers found
} ThreadTask;

/**
 * Function to verify if a number is prime.
 * Uses an optimized trial division method.
 * 
 * @param num Integer number to check
 * @return 1 if prime, 0 otherwise
 */
int check_prime(int num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return 0;
    }
    return 1;
}

/**
 * Thread function to analyze a file for prime numbers.
 * Writes identified prime numbers to "Only_prime.txt".
 * 
 * @param arg Pointer to ThreadTask structure
 * @return NULL
 */
void *analyze_file(void *arg) {
    ThreadTask *data = (ThreadTask *)arg;
    FILE *file = fopen(data->filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", data->filename);
        data->prime_count = 0;
        return NULL;
    }

    FILE *prime_output = fopen("Only_prime.txt", "a");  // Open output file in append mode
    if (!prime_output) {
        printf("Error opening output file: Only_prime.txt\n");
        fclose(file);
        data->prime_count = 0;
        return NULL;
    }

    int num;
    data->prime_count = 0;
    
    while (fscanf(file, "%d", &num) == 1) {
        if (check_prime(num)) {
            data->prime_count++;
            fprintf(prime_output, "%d\n", num);  // Write prime number to output file
        }
    }

    fclose(file);
    fclose(prime_output);
    return NULL;
}

int main() {
    // List of input files containing numbers
    const char *filenames[] = {
        "PrimeData1.txt",
        "PrimeData2.txt",
        "PrimeData3.txt"
    };
    
    int num_files = 3;
    pthread_t threads[num_files];
    ThreadTask thread_data[num_files];

    // Clear the output file before starting
    FILE *clear_file = fopen("Only_prime.txt", "w");
    if (clear_file) {
        fclose(clear_file);
    } else {
        printf("Error clearing output file: Only_prime.txt\n");
        return 1;
    }

    // Create threads to process each file
    for (int i = 0; i < num_files; i++) {
        thread_data[i].filename = filenames[i];
        if (pthread_create(&threads[i], NULL, analyze_file, (void *)&thread_data[i]) != 0) {
            printf("Error creating thread for file: %s\n", filenames[i]);
        }
    }

    int total_primes = 0;

    // Wait for threads to finish and collect results
    for (int i = 0; i < num_files; i++) {
        pthread_join(threads[i], NULL);
        total_primes += thread_data[i].prime_count;
        printf("File: %s, Prime Count: %d\n", thread_data[i].filename, thread_data[i].prime_count);
    }

    printf("Total Prime Count from all files: %d\n", total_primes);
    printf("Prime numbers have been saved in Only_prime.txt\n");

    return 0;
}
