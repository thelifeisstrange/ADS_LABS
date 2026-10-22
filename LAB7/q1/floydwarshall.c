/*
 * Lab Set 5 Q2: All-pairs shortest paths with the Floyd-Warshall algorithm.
 */
#include <stdio.h>

#define V 4
#define INF 99999

static void printSolution(int matrix[][V]) {
    printf("Shortest distances between every pair of vertices:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (matrix[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", matrix[i][j]);
        }
        printf("\n");
    }
}

static void floydWarshall(int graph[][V]) {
    int matrix[V][V];

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            matrix[i][j] = graph[i][j];

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (matrix[i][k] != INF && matrix[k][j] != INF &&
                    matrix[i][k] + matrix[k][j] < matrix[i][j])
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
            }
        }
    }

    for (int i = 0; i < V; i++) {
        if (matrix[i][i] < 0) {
            printf("Error: Graph contains a negative weight cycle!\n");
            return;
        }
    }
    printSolution(matrix);
}

int main(void) {
    int graph[V][V] = {
        {0, 5, INF, 10},
        {INF, 0, 3, INF},
        {INF, INF, 0, 1},
        {INF, INF, INF, 0},
    };

    floydWarshall(graph);
    return 0;
}
