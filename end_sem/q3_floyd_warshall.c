#include <stdio.h>
#define INF 99999
#define MAX 10

int main() {
    int n, d[MAX][MAX];
    printf("Number of vertices: ");
    scanf("%d", &n);
    printf("Enter cost matrix (%d for no edge):\n", INF);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &d[i][j]);

    // try every vertex k as an intermediate stop between i and j
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (d[i][k] + d[k][j] < d[i][j])
                    d[i][j] = d[i][k] + d[k][j];

    printf("\nAll-pairs shortest paths:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (d[i][j] >= INF) printf("%5s", "INF");
            else                printf("%5d", d[i][j]);
        }
        printf("\n");
    }
    return 0;
}
