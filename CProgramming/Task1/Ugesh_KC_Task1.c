#include <stdio.h>
#include <stdlib.h>

// Structure to represent a point with x and y coordinates
typedef struct {
    double x;
    double y;
} Point;

/**
 * Reads data from a file and stores it in an array of Point structures.
 * 
 * @param filename Name of the file to read from.
 * @param points Array to store the data.
 * @param max_points Maximum number of points to read.
 * @return Number of points successfully read from the file, or -1 if an error occurs.
 */
int read_data(const char *filename, Point *points, int max_points) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return -1; // Return error if file opening fails
    }

    int count = 0;
    // Read (x, y) pairs from the file, assuming they are comma-separated
    while (fscanf(file, "%lf,%lf", &points[count].x, &points[count].y) == 2) {
        count++;
        if (count >= max_points) break; // Stop if the max limit is reached
    }
    fclose(file); // Close the file after reading
    return count; // Return the number of data points read
}

/**
 * Computes the coefficients of a simple linear regression model (y = bx + a).
 * Uses the least squares method to find the best-fitting line.
 * 
 * @param points Array of points (x, y) used for regression.
 * @param n Number of points in the dataset.
 * @param a Pointer to store the computed intercept.
 * @param b Pointer to store the computed slope.
 */
void compute_lr(Point *points, int n, double *a, double *b) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

    // Compute the sums required for the regression formula
    for (int i = 0; i < n; i++) {
        sum_x += points[i].x;
        sum_y += points[i].y;
        sum_xy += points[i].x * points[i].y;
        sum_x2 += points[i].x * points[i].x;
    }

    // Compute the slope (b) and intercept (a) using least squares formula
    *b = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    *a = (sum_y - (*b) * sum_x) / n;
}

int main() {
    // List of dataset filenames to process
    const char *filenames[] = {
        "datasetLR1.txt",
        "datasetLR2.txt",
        "datasetLR3.txt",
        "datasetLR4.txt"
    };

    int max_points_per_file = 1000; // Maximum number of data points per file
    int total_points = 0; // Counter for total data points from all files

    // Allocate memory for storing all data points (up to 4000 points)
    Point *all_points = (Point *)malloc(4000 * sizeof(Point));
    if (all_points == NULL) {
        printf("Memory allocation failed\n");
        return 1; // Exit if memory allocation fails
    }

    // Process each file individually
    for (int i = 0; i < 4; i++) {
        // Allocate temporary storage for points in the current file
        Point *file_points = (Point *)malloc(max_points_per_file * sizeof(Point));
        if (file_points == NULL) {
            printf("Memory allocation failed\n");
            return 1; // Exit if memory allocation fails
        }

        // Read data from the file
        int count = read_data(filenames[i], file_points, max_points_per_file);
        if (count < 2) {
            printf("Not enough data in %s to compute regression.\n", filenames[i]);
            free(file_points); // Free allocated memory
            continue; // Skip this file and move to the next
        }

        // Compute linear regression for the current dataset
        double a, b;
        compute_lr(file_points, count, &a, &b);
        printf("\nFile: %s\n", filenames[i]);
        printf("Computed Linear Regression: y = %.2fx + %.2f\n", b, a);

        // Merge individual file data into the global dataset
        for (int j = 0; j < count; j++) {
            all_points[total_points++] = file_points[j];
        }

        free(file_points); // Free memory allocated for this file's data
    }

    // Compute linear regression on the combined dataset
    if (total_points >= 2) {
        double final_a, final_b;
        compute_lr(all_points, total_points, &final_a, &final_b);
        printf("\n Final Combined Linear Regression (All Files) \n");
        printf("y = %.2fx + %.2f\n", final_b, final_a);

        // Prompt the user to enter a value for prediction
        double x_input;
        printf("Enter a value for x: ");
        scanf("%lf", &x_input);
        printf("Predicted y: %.2f\n", final_b * x_input + final_a);
    } else {
        printf("Not enough data for combined regression.\n");
    }

    free(all_points); // Free allocated memory before exiting
    return 0;
}
