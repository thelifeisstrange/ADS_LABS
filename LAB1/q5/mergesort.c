#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Merge function to combine two sorted sub-arrays
void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    // Copy data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temp arrays back into arr[left..right]
    i = 0; 
    j = 0; 
    k = left; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Free temporary arrays
    free(L);
    free(R);
}

// Main Merge Sort function
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

int main() {
    // Define a single large exponential input size directly (e.g., 10^6)
    long long n = 500000; 

    printf("=======================================\n");
    printf("   SINGLE EXPONENTIAL MERGE SORT TEST  \n");
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

    printf("Sorting using Merge Sort...\n");

    // Capture start time
    clock_t start_time = clock();

    // Perform Merge Sort
    mergeSort(arr, 0, n - 1);

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