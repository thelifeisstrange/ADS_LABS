#include <stdio.h>
#include <pthread.h>
#define MAX 1000

int a[MAX], tmp[MAX];

typedef struct { int l, r, depth; } Task;

void merge(int l, int m, int r) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r)
        tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (k = l; k <= r; k++) a[k] = tmp[k];
}

void *mergeSort(void *arg) {
    Task *t = (Task *)arg;
    if (t->l >= t->r) return NULL;
    int m = (t->l + t->r) / 2;
    Task left  = {t->l, m,     t->depth + 1};
    Task right = {m + 1, t->r, t->depth + 1};

    if (t->depth < 3) {                       // spawn a thread only for the top levels
        pthread_t th;
        pthread_create(&th, NULL, mergeSort, &left);  // left half in a new thread
        mergeSort(&right);                            // right half in this thread
        pthread_join(th, NULL);                       // wait before merging
    } else {                                  // deeper levels: plain recursion
        mergeSort(&left);
        mergeSort(&right);
    }
    merge(t->l, m, t->r);
    return NULL;
}

int main() {
    int n;
    printf("Enter N: ");
    scanf("%d", &n);
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);

    Task root = {0, n - 1, 0};
    mergeSort(&root);

    printf("Sorted: ");
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
    return 0;
}
