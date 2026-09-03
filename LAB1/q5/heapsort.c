#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// A utility function to swap two elements
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// To heapify a subtree rooted with node i which is an index in arr[]. 
// n is the size of the heap
void heapify(int arr[], int n, int i) {
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // left = 2*i + 1
    int right = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If largest is not root
    if (largest != i) {
        swap(&arr[i], &arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// Main function to do heap sort
void heapSort(int arr[], int n) {
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        swap(&arr[0], &arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

int main() {
    // Define a single large exponential input size directly (e.g., 10^6)
    long long n = 500000; 

    printf("=======================================\n");
    printf("   SINGLE EXPONENTIAL HEAP SORT TEST   \n");
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

    printf("Sorting using Heap Sort...\n");

    // Capture start time
    clock_t start_time = clock();

    // Perform Heap Sort
    heapSort(arr, n);

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