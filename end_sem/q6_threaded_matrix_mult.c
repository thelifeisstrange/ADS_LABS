#include <stdio.h>
#include <pthread.h>
#define MAX 10

int N, A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

// each thread computes one full row of C
void *computeRow(void *arg) {
    int i = *(int *)arg;
    for (int j = 0; j < N; j++) {
        C[i][j] = 0;
        for (int k = 0; k < N; k++)
            C[i][j] += A[i][k] * B[k][j];
    }
    return NULL;
}

int main() {
    printf("Enter N: ");
    scanf("%d", &N);
    printf("Enter matrix A:\n");
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) scanf("%d", &A[i][j]);
    printf("Enter matrix B:\n");
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) scanf("%d", &B[i][j]);

    pthread_t th[MAX];
    int row[MAX];
    for (int i = 0; i < N; i++) {
        row[i] = i;                                   // separate variable per thread
        pthread_create(&th[i], NULL, computeRow, &row[i]);
    }
    for (int i = 0; i < N; i++) pthread_join(th[i], NULL);

    printf("Result C = A * B:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) printf("%5d", C[i][j]);
        printf("\n");
    }
    return 0;
}
