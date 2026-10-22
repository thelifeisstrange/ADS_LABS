/*
 * Lab Set 6 Q1: Johnson's algorithm — all-pairs shortest paths on a sparse
 * directed graph with positive and negative edges (no negative cycles).
 */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INF LLONG_MAX

typedef long long ll;

typedef struct {
    int u, v;
    ll w;
} Edge;

typedef struct {
    int v;
    ll w;
    int next;
} AdjEdge;

typedef struct {
    ll dist;
    int vertex;
} HeapNode;

typedef struct {
    HeapNode *arr;
    int size;
    int capacity;
} MinHeap;

static void swap(HeapNode *a, HeapNode *b) {
    HeapNode t = *a;
    *a = *b;
    *b = t;
}

static MinHeap *createHeap(int capacity) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (!heap)
        return NULL;
    heap->arr = (HeapNode *)malloc((size_t)capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

static void push(MinHeap *heap, HeapNode node) {
    if (heap->size >= heap->capacity) {
        heap->capacity *= 2;
        HeapNode *temp =
            (HeapNode *)realloc(heap->arr, (size_t)heap->capacity * sizeof(HeapNode));
        if (!temp)
            return;
        heap->arr = temp;
    }
    int i = heap->size++;
    heap->arr[i] = node;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->arr[parent].dist <= heap->arr[i].dist)
            break;
        swap(&heap->arr[parent], &heap->arr[i]);
        i = parent;
    }
}

static HeapNode pop(MinHeap *heap) {
    HeapNode result = heap->arr[0];
    heap->arr[0] = heap->arr[--heap->size];
    int i = 0;
    for (;;) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist)
            smallest = left;
        if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist)
            smallest = right;
        if (smallest == i)
            break;
        swap(&heap->arr[i], &heap->arr[smallest]);
        i = smallest;
    }
    return result;
}

static int isEmpty(MinHeap *heap) { return heap->size == 0; }

int main(void) {
    int V, E;

    printf("\nEnter number of vertices and edges (e.g., 5 8): ");
    fflush(stdout);
    if (scanf("%d %d", &V, &E) != 2 || V < 1 || E < 0)
        return 0;

    Edge *edges = (Edge *)malloc((size_t)(E + V) * sizeof(Edge));
    AdjEdge *adj = (AdjEdge *)malloc((size_t)(E > 0 ? E : 1) * sizeof(AdjEdge));
    int *head = (int *)malloc((size_t)V * sizeof(int));
    ll *h = (ll *)malloc((size_t)(V + 1) * sizeof(ll));
    ll **dist = (ll **)malloc((size_t)V * sizeof(ll *));

    if (!edges || !adj || !head || !h || !dist)
        return 0;

    for (int i = 0; i < V; i++)
        head[i] = -1;

    printf("\nEnter the %d edges as: source destination weight\n", E);
    fflush(stdout);
    int uses_one_indexing = 0;
    for (int i = 0; i < E; i++) {
        if (scanf("%d %d %lld", &edges[i].u, &edges[i].v, &edges[i].w) != 3)
            return 0;
        if (edges[i].u == V || edges[i].v == V)
            uses_one_indexing = 1;
    }
    if (uses_one_indexing) {
        for (int i = 0; i < E; i++) {
            edges[i].u--;
            edges[i].v--;
        }
    }

    int S = V;
    for (int i = 0; i < V; i++) {
        edges[E + i].u = S;
        edges[E + i].v = i;
        edges[E + i].w = 0;
    }
    int totalEdges = E + V;

    for (int i = 0; i <= V; i++)
        h[i] = INF;
    h[S] = 0;

    for (int i = 0; i < V; i++) {
        int updated = 0;
        for (int j = 0; j < totalEdges; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            ll w = edges[j].w;
            if (h[u] != INF && h[u] + w < h[v]) {
                h[v] = h[u] + w;
                updated = 1;
            }
        }
        if (!updated)
            break;
    }

    for (int j = 0; j < totalEdges; j++) {
        int u = edges[j].u;
        int v = edges[j].v;
        ll w = edges[j].w;
        if (h[u] != INF && h[u] + w < h[v]) {
            printf("\nNegative weight cycle detected!\n");
            free(edges);
            free(adj);
            free(head);
            free(h);
            free(dist);
            return 0;
        }
    }

    for (int i = 0; i < E; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        adj[i].v = v;
        adj[i].w = edges[i].w + h[u] - h[v];
        adj[i].next = head[u];
        head[u] = i;
    }

    for (int i = 0; i < V; i++) {
        dist[i] = (ll *)malloc((size_t)V * sizeof(ll));
        for (int j = 0; j < V; j++)
            dist[i][j] = INF;
    }

    for (int src = 0; src < V; src++) {
        ll *d = (ll *)malloc((size_t)V * sizeof(ll));
        for (int i = 0; i < V; i++)
            d[i] = INF;

        MinHeap *heap = createHeap(E > 0 ? E : 1);
        d[src] = 0;
        push(heap, (HeapNode){0, src});

        while (!isEmpty(heap)) {
            HeapNode current = pop(heap);
            ll du = current.dist;
            int u = current.vertex;
            if (du != d[u])
                continue;
            for (int i = head[u]; i != -1; i = adj[i].next) {
                int v = adj[i].v;
                ll w = adj[i].w;
                if (d[u] != INF && d[u] + w < d[v]) {
                    d[v] = d[u] + w;
                    push(heap, (HeapNode){d[v], v});
                }
            }
        }

        for (int v = 0; v < V; v++) {
            if (d[v] != INF)
                dist[src][v] = d[v] - h[src] + h[v];
        }
        free(d);
        free(heap->arr);
        free(heap);
    }

    printf("\nShortest Path Distance Matrix:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                printf("INF ");
            else
                printf("%lld ", dist[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < V; i++)
        free(dist[i]);
    free(dist);
    free(edges);
    free(adj);
    free(head);
    free(h);
    return 0;
}
