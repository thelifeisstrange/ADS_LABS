/*
 * Lab Set 9 Q2: Multithreaded merge sort — recursive split with pthreads on large ranges.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100000
#define THREAD_THRESHOLD 4096

typedef struct {
    int *arr;
    int left;
    int right;
} SortArg;

static void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = (int *)malloc((size_t)n1 * sizeof(int));
    int *R = (int *)malloc((size_t)n2 * sizeof(int));
    if (!L || !R)
        exit(1);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

static void merge_sort_serial(int arr[], int left, int right) {
    if (left >= right)
        return;
    int mid = left + (right - left) / 2;
    merge_sort_serial(arr, left, mid);
    merge_sort_serial(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

static void merge_sort_mt(int arr[], int left, int right);

static void *sort_thread(void *arg) {
    SortArg *task = (SortArg *)arg;
    merge_sort_mt(task->arr, task->left, task->right);
    return NULL;
}

static void merge_sort_mt(int arr[], int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    int len = right - left + 1;

    if (len > THREAD_THRESHOLD) {
        SortArg left_task = {arr, left, mid};
        SortArg right_task = {arr, mid + 1, right};
        pthread_t t1, t2;

        if (pthread_create(&t1, NULL, sort_thread, &left_task) != 0 ||
            pthread_create(&t2, NULL, sort_thread, &right_task) != 0) {
            merge_sort_serial(arr, left, right);
            return;
        }
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    } else {
        merge_sort_serial(arr, left, mid);
        merge_sort_serial(arr, mid + 1, right);
    }
    merge(arr, left, mid, right);
}

int main(void) {
    int n;
    printf("Enter array size N: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        fprintf(stderr, "Invalid N (use 1..%d).\n", MAX_N);
        return 1;
    }

    int *arr = (int *)malloc((size_t)n * sizeof(int));
    if (!arr)
        return 1;

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1)
            return 1;
    }

    printf("Input array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    merge_sort_mt(arr, 0, n - 1);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    free(arr);
    return 0;
}
