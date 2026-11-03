#include <stdio.h>
#define MAX 100

int a[MAX], tmp[MAX];

void merge(int l, int m, int r) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r)
        tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (k = l; k <= r; k++) a[k] = tmp[k];
}

void mergeSort(int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(l, m);
    mergeSort(m + 1, r);
    merge(l, m, r);
}

int main() {
    int n;
    printf("Enter n: ");
    scanf("%d", &n);
    printf("Enter %d numbers: ", n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);

    mergeSort(0, n - 1);

    printf("Sorted: ");
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
    return 0;
}
