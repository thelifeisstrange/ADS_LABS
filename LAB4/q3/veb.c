#include <stdio.h>
#include <stdlib.h>

#define NIL -1

typedef struct VEB {
    int u;
    int min;
    int max;
    struct VEB *summary;
    struct VEB **cluster;
} VEB;

int upperSqrt(int u) {
    int lg = 0, t = u;
    while (t > 1) {
        t >>= 1;
        lg++;
    }
    int half = (lg + 1) / 2;
    return 1 << half;
}

int lowerSqrt(int u) {
    int lg = 0, t = u;
    while (t > 1) {
        t >>= 1;
        lg++;
    }
    int half = lg / 2;
    return 1 << half;
}

int high(VEB *V, int x) { return x / lowerSqrt(V->u); }
int low(VEB *V, int x) { return x % lowerSqrt(V->u); }
int indexOf(VEB *V, int h, int l) { return h * lowerSqrt(V->u) + l; }

VEB *createVEB(int u) {
    VEB *V = (VEB *)malloc(sizeof(VEB));
    V->u = u;
    V->min = NIL;
    V->max = NIL;
    V->summary = NULL;
    V->cluster = NULL;
    if (u > 2) {
        int clusters = upperSqrt(u);
        int cu = lowerSqrt(u);
        V->summary = createVEB(clusters);
        V->cluster = (VEB **)malloc(clusters * sizeof(VEB *));
        for (int i = 0; i < clusters; i++)
            V->cluster[i] = createVEB(cu);
    }
    return V;
}

int member(VEB *V, int x) {
    if (x == V->min || x == V->max)
        return 1;
    if (V->u == 2)
        return 0;
    return member(V->cluster[high(V, x)], low(V, x));
}

void vebInsert(VEB *V, int x) {
    if (V->min == NIL) {
        V->min = V->max = x;
        return;
    }
    if (x < V->min) {
        int tmp = V->min;
        V->min = x;
        x = tmp;
    }
    if (V->u > 2) {
        int h = high(V, x);
        int l = low(V, x);
        if (V->cluster[h]->min == NIL)
            vebInsert(V->summary, h);
        vebInsert(V->cluster[h], l);
    }
    if (x > V->max)
        V->max = x;
}

int successor(VEB *V, int x) {
    if (V->u == 2) {
        if (x == 0 && V->max == 1)
            return 1;
        return NIL;
    }
    if (V->min != NIL && x < V->min)
        return V->min;
    int h = high(V, x);
    int l = low(V, x);
    int maxLow = V->cluster[h]->max;
    if (maxLow != NIL && l < maxLow) {
        int s = successor(V->cluster[h], l);
        return indexOf(V, h, s);
    }
    int succCluster = successor(V->summary, h);
    if (succCluster == NIL)
        return NIL;
    int minLow = V->cluster[succCluster]->min;
    return indexOf(V, succCluster, minLow);
}

int predecessor(VEB *V, int x) {
    if (V->u == 2) {
        if (x == 1 && V->min == 0)
            return 0;
        return NIL;
    }
    if (V->max != NIL && x > V->max)
        return V->max;
    int h = high(V, x);
    int l = low(V, x);
    int minLow = V->cluster[h]->min;
    if (minLow != NIL && l > minLow) {
        int p = predecessor(V->cluster[h], l);
        return indexOf(V, h, p);
    }
    int predCluster = predecessor(V->summary, h);
    if (predCluster == NIL) {
        if (V->min != NIL && x > V->min)
            return V->min;
        return NIL;
    }
    int maxLow = V->cluster[predCluster]->max;
    return indexOf(V, predCluster, maxLow);
}

void vebDelete(VEB *V, int x) {
    if (V->min == V->max) {
        V->min = V->max = NIL;
        return;
    }
    if (V->u == 2) {
        if (x == 0)
            V->min = 1;
        else
            V->min = 0;
        V->max = V->min;
        return;
    }
    if (x == V->min) {
        int first = V->summary->min;
        x = indexOf(V, first, V->cluster[first]->min);
        V->min = x;
    }
    vebDelete(V->cluster[high(V, x)], low(V, x));
    if (V->cluster[high(V, x)]->min == NIL)
        vebDelete(V->summary, high(V, x));
    if (x == V->max) {
        if (V->summary->max == NIL)
            V->max = V->min;
        else {
            int last = V->summary->max;
            V->max = indexOf(V, last, V->cluster[last]->max);
        }
    }
}

void printVEB(VEB *V, int indent, const char *label) {
    for (int i = 0; i < indent; i++) printf("  ");
    printf("%s u=%d  min=", label, V->u);
    if (V->min == NIL) printf("NIL");
    else printf("%d", V->min);
    printf("  max=");
    if (V->max == NIL) printf("NIL");
    else printf("%d", V->max);
    printf("\n");
    if (V->u == 2)
        return;
    printVEB(V->summary, indent + 1, "summary");
    int clusters = upperSqrt(V->u);
    for (int i = 0; i < clusters; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "cluster[%d]", i);
        printVEB(V->cluster[i], indent + 1, buf);
    }
}

int main(void) {
    VEB *V = createVEB(16);

    printf("============================================================\n");
    printf("  LAB 4 Q3: van Emde Boas TREE  (U = 16)\n");
    printf("============================================================\n");
    printf("Recursive layout: u=16 -> 4 clusters of size 4, plus summary.\n");
    printf("Each cluster of size 4 -> 2 clusters of size 2.\n\n");

    printVEB(V, 0, "empty tree");

    int keys[] = {2, 3, 4, 5, 7, 14, 15};
    int n = (int)(sizeof(keys) / sizeof(keys[0]));
    for (int i = 0; i < n; i++) {
        printf("\n>>> Insert(%d)\n", keys[i]);
        vebInsert(V, keys[i]);
        printVEB(V, 0, "root");
    }

    printf("\n--- Member ---\n");
    for (int x = 0; x < 16; x++)
        if (member(V, x))
            printf("Member(%d) = YES\n", x);
    printf("Member(6) = %s\n", member(V, 6) ? "YES" : "NO");
    printf("Member(10) = %s\n", member(V, 10) ? "YES" : "NO");

    printf("\n--- Successor / Predecessor ---\n");
    int queries[] = {1, 5, 6, 7, 14, 15};
    for (int i = 0; i < 6; i++) {
        int x = queries[i];
        int s = successor(V, x);
        int p = predecessor(V, x);
        printf("x=%2d  successor=", x);
        if (s == NIL) printf("NIL");
        else printf("%d", s);
        printf("  predecessor=");
        if (p == NIL) printf("NIL\n");
        else printf("%d\n", p);
    }

    printf("\n>>> Delete(5)\n");
    vebDelete(V, 5);
    printVEB(V, 0, "root");
    printf("Member(5) = %s\n", member(V, 5) ? "YES" : "NO");
    printf("Successor(4) = ");
    int s = successor(V, 4);
    if (s == NIL) printf("NIL\n");
    else printf("%d\n", s);

    printf("\n>>> Delete(2)\n");
    vebDelete(V, 2);
    printVEB(V, 0, "root");

    printf("\n============================================================\n");
    printf("Operations take O(log log U) = O(log log 16) = O(2) steps.\n");
    printf("============================================================\n");
    return 0;
}
