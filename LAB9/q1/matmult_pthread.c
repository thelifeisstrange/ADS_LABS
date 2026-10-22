/*
 * Lab Set 9 Q1: Multithreaded matrix multiplication (one pthread per row of C).
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 512

typedef struct {
    int row;
    int n;
    const int *a;
    const int *b;
    int *c;
} RowArg;

static void *multiply_row(void *arg) {
    RowArg *task = (RowArg *)arg;
    int row = task->row;
    int n = task->n;
    const int *a = task->a;
    const int *b = task->b;
    int *c = task->c;

    for (int j = 0; j < n; j++) {
        long sum = 0;
        for (int k = 0; k < n; k++)
            sum += (long)a[row * n + k] * (long)b[k * n + j];
        c[row * n + j] = (int)sum;
    }
    return NULL;
}

static void print_matrix(const char *name, const int *m, int n) {
    printf("%s:\n", name);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", m[i * n + j]);
        printf("\n");
    }
}

int main(void) {
    int n;
    printf("Enter matrix size N: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_N) {
        fprintf(stderr, "Invalid N (use 1..%d).\n", MAX_N);
        return 1;
    }

    int *a = (int *)malloc((size_t)n * n * sizeof(int));
    int *b = (int *)malloc((size_t)n * n * sizeof(int));
    int *c = (int *)calloc((size_t)n * n, sizeof(int));
    if (!a || !b || !c)
        return 1;

    printf("Enter %d elements of matrix A (row by row):\n", n * n);
    for (int i = 0; i < n * n; i++) {
        if (scanf("%d", &a[i]) != 1)
            return 1;
    }
    printf("Enter %d elements of matrix B (row by row):\n", n * n);
    for (int i = 0; i < n * n; i++) {
        if (scanf("%d", &b[i]) != 1)
            return 1;
    }

    pthread_t *threads = (pthread_t *)malloc((size_t)n * sizeof(pthread_t));
    RowArg *args = (RowArg *)malloc((size_t)n * sizeof(RowArg));
    if (!threads || !args)
        return 1;

    for (int i = 0; i < n; i++) {
        args[i].row = i;
        args[i].n = n;
        args[i].a = a;
        args[i].b = b;
        args[i].c = c;
        if (pthread_create(&threads[i], NULL, multiply_row, &args[i]) != 0) {
            fprintf(stderr, "pthread_create failed for row %d\n", i);
            return 1;
        }
    }
    for (int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);

    print_matrix("Matrix A", a, n);
    print_matrix("Matrix B", b, n);
    print_matrix("Matrix C = A * B", c, n);

    free(a);
    free(b);
    free(c);
    free(threads);
    free(args);
    return 0;
}
