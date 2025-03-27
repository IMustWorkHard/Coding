#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lodepng.h"

// Function prototypes
void* apply_blur(void* arg);
void blur_pixel(unsigned char* image, int x, int y, int width, int height, int channels);
void save_image(const char* filename, unsigned char* image, int width, int height, int channels);

// Global variables
unsigned char* image_data;
int image_width, image_height, image_channels;

// Number of threads
int thread_count = 4;

// Thread arguments
typedef struct {
    int start_row;
    int end_row;
} ThreadArgs;

// Function to apply simple blur to a single pixel
void blur_pixel(unsigned char* image, int x, int y, int width, int height, int channels) {
    int sum_r = 0, sum_g = 0, sum_b = 0, count = 0;

    // Loop over a small 3x3 neighborhood
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int nx = x + i;
            int ny = y + j;

            // Check bounds
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                int index = (ny * width + nx) * channels;
                sum_r += image[index];
                sum_g += image[index + 1];
                sum_b += image[index + 2];
                count++;
            }
        }
    }

    // Calculate average
    int index = (y * width + x) * channels;
    image[index] = sum_r / count;
    image[index + 1] = sum_g / count;
    image[index + 2] = sum_b / count;
}

// Function to apply blur to a range of rows (executed by each thread)
void* apply_blur(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int start_row = args->start_row;
    int end_row = args->end_row;

    for (int y = start_row; y < end_row; y++) {
        for (int x = 0; x < image_width; x++) {
            blur_pixel(image_data, x, y, image_width, image_height, image_channels);
        }
    }

    pthread_exit(NULL);
}

// Function to save the blurred image
void save_image(const char* filename, unsigned char* image, int width, int height, int channels) {
    unsigned error = lodepng_encode32_file(filename, image, width, height);
    if (error) {
        printf("Error saving image: %s\n", lodepng_error_text(error));
    } else {
        printf("Blurred image saved as %s\n", filename);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_image> <output_image>\n", argv[0]);
        return 1;
    }

    // Load image
    unsigned error = lodepng_decode32_file(&image_data, &image_width, &image_height, argv[1]);
    if (error) {
        printf("Error loading image: %s\n", lodepng_error_text(error));
        return 1;
    }

    image_channels = 4; // Assuming RGBA format

    // Create threads
    pthread_t threads[thread_count];
    ThreadArgs args[thread_count];

    int rows_per_thread = image_height / thread_count;
    for (int i = 0; i < thread_count; i++) {
        args[i].start_row = i * rows_per_thread;
        args[i].end_row = (i == thread_count - 1) ? image_height : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, apply_blur, &args[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Save blurred image
    save_image(argv[2], image_data, image_width, image_height, image_channels);

    // Free memory
    free(image_data);

    return 0;
}
