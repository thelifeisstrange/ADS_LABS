#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// A utility function to swap two elements
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Partition function for Quicksort
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Pivot element
    int i = (low - 1);     // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Main Quicksort function
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    // Define a single large exponential input size directly (e.g., 10^6)
    long long n = 500000; 

    printf("=======================================\n");
    printf("   SINGLE EXPONENTIAL QUICKSORT TEST   \n");
    printf("=======================================\n");
    printf("Target Size (n): %lld\n", n);

    // Dynamically allocate memory for the large array
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Seed random number generator
    srand(time(NULL));

    // Populate array with random numbers
    printf("Generating %lld random numbers...\n", n);
    for (long long i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }

    printf("Sorting using Quicksort...\n");

    // Capture start time
    clock_t start_time = clock();

    // Perform Quicksort
    quickSort(arr, 0, n - 1);

    // Capture end time
    clock_t end_time = clock();

    // Calculate elapsed time in seconds
    double cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("\n--- RESULT ---\n");
    printf("Successfully sorted %lld elements.\n", n);
    printf("Time taken: %.6f seconds\n", cpu_time_used);

    // Free the dynamically allocated memory
    free(arr);

    return 0;
}