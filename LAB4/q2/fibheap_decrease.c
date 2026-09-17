#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Node {
    int key;
    int degree;
    int mark;
    struct Node *parent;
    struct Node *child;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *min;
    int n;
} FibHeap;

Node *createNode(int key) {
    Node *x = (Node *)malloc(sizeof(Node));
    x->key = key;
    x->degree = 0;
    x->mark = 0;
    x->parent = NULL;
    x->child = NULL;
    x->left = x;
    x->right = x;
    return x;
}

void initHeap(FibHeap *H) {
    H->min = NULL;
    H->n = 0;
}

Node *concatLists(Node *a, Node *b) {
    if (!a) return b;
    if (!b) return a;
    Node *a_right = a->right;
    Node *b_left = b->left;
    a->right = b;
    b->left = a;
    a_right->left = b_left;
    b_left->right = a_right;
    return a;
}

void insertRoot(FibHeap *H, Node *x) {
    x->parent = NULL;
    if (!H->min) {
        x->left = x->right = x;
        H->min = x;
    } else {
        concatLists(H->min, x);
        if (x->key < H->min->key)
            H->min = x;
    }
}

Node *insert(FibHeap *H, int key) {
    Node *x = createNode(key);
    insertRoot(H, x);
    H->n++;
    return x;
}

void printSubtree(Node *x, int indent) {
    if (!x) return;
    Node *start = x;
    do {
        for (int i = 0; i < indent; i++) printf("  ");
        printf("- %d  (deg=%d, mark=%s)\n", start->key, start->degree, start->mark ? "TRUE" : "false");
        if (start->child)
            printSubtree(start->child, indent + 1);
        start = start->right;
    } while (start != x);
}

void displayHeap(const char *title, FibHeap *H) {
    printf("\n--- %s ---\n", title);
    printf("n = %d\n", H->n);
    if (!H->min) {
        printf("Heap empty.\n");
        return;
    }
    printf("Find-Min: %d\n", H->min->key);
    printf("Root list: ");
    Node *r = H->min;
    do {
        printf("%d", r->key);
        r = r->right;
        if (r != H->min) printf(" <-> ");
    } while (r != H->min);
    printf("\nForest:\n");
    r = H->min;
    int tree = 1;
    do {
        printf("  Tree %d:\n", tree++);
        printf("    - %d  (deg=%d, mark=%s)\n", r->key, r->degree, r->mark ? "TRUE" : "false");
        if (r->child)
            printSubtree(r->child, 3);
        r = r->right;
    } while (r != H->min);
}

Node *findInList(Node *start, int key) {
    if (!start) return NULL;
    Node *cur = start;
    do {
        if (cur->key == key)
            return cur;
        Node *found = findInList(cur->child, key);
        if (found) return found;
        cur = cur->right;
    } while (cur != start);
    return NULL;
}

Node *findKey(FibHeap *H, int key) {
    return findInList(H->min, key);
}

void removeFromList(Node *x) {
    x->left->right = x->right;
    x->right->left = x->left;
    x->left = x->right = x;
}

void fibLink(Node *y, Node *x) {
    removeFromList(y);
    y->parent = x;
    if (!x->child) {
        x->child = y;
        y->left = y->right = y;
    } else {
        concatLists(x->child, y);
    }
    x->degree++;
    y->mark = 0;
}

void consolidate(FibHeap *H) {
    if (!H->min) return;
    Node *A[64];
    int i;
    for (i = 0; i < 64; i++) A[i] = NULL;

    Node *start = H->min;
    Node *w = start;
    Node *roots[64];
    int nroots = 0;
    do {
        roots[nroots++] = w;
        w = w->right;
    } while (w != start);

    for (i = 0; i < nroots; i++) {
        Node *x = roots[i];
        int d = x->degree;
        while (A[d]) {
            Node *y = A[d];
            if (x->key > y->key) {
                Node *tmp = x;
                x = y;
                y = tmp;
            }
            fibLink(y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    }

    H->min = NULL;
    for (i = 0; i < 64; i++) {
        if (!A[i]) continue;
        A[i]->left = A[i]->right = A[i];
        A[i]->parent = NULL;
        insertRoot(H, A[i]);
    }
}

Node *extractMin(FibHeap *H) {
    Node *z = H->min;
    if (!z) return NULL;

    if (z->child) {
        Node *c = z->child;
        Node *start = c;
        do {
            Node *next = c->right;
            c->parent = NULL;
            c = next;
        } while (c != start);
        concatLists(H->min, z->child);
        z->child = NULL;
    }

    if (z->right == z) {
        H->min = NULL;
    } else {
        H->min = z->right;
        removeFromList(z);
        consolidate(H);
    }
    H->n--;
    z->left = z->right = z;
    z->parent = z->child = NULL;
    return z;
}

void cut(FibHeap *H, Node *x, Node *y) {
    if (x->right == x)
        y->child = NULL;
    else {
        if (y->child == x)
            y->child = x->right;
        removeFromList(x);
    }
    y->degree--;
    x->parent = NULL;
    x->mark = 0;
    insertRoot(H, x);
    printf("  CUT: moved %d from child-list of %d to the root list\n", x->key, y->key);
}

void cascadingCut(FibHeap *H, Node *y) {
    Node *z = y->parent;
    if (!z) return;
    if (!y->mark) {
        y->mark = 1;
        printf("  MARK: node %d is now marked (lost one child)\n", y->key);
    } else {
        printf("  CASCADE: node %d was already marked — cut and continue\n", y->key);
        cut(H, y, z);
        cascadingCut(H, z);
    }
}

int decreaseKey(FibHeap *H, Node *x, int k) {
    if (!x || k > x->key) {
        printf("Decrease-Key error\n");
        return 0;
    }
    printf("Decrease-Key: %d -> %d\n", x->key, k);
    x->key = k;
    Node *y = x->parent;
    if (y && x->key < y->key) {
        cut(H, x, y);
        cascadingCut(H, y);
    }
    if (x->key < H->min->key)
        H->min = x;
    return 1;
}

void deleteNode(FibHeap *H, Node *x) {
    int original = x->key;
    printf("Delete node %d: Decrease-Key to -INF, then Extract-Min\n", original);
    decreaseKey(H, x, INT_MIN);
    Node *z = extractMin(H);
    if (z) {
        printf("Extract-Min removed the deleted node (was %d)\n", original);
        free(z);
    }
}

int main(void) {
    FibHeap H;
    initHeap(&H);

    printf("============================================================\n");
    printf("  LAB 4 Q2: FIBONACCI HEAP — DECREASE-KEY AND DELETE\n");
    printf("============================================================\n");

    int keys[] = {23, 7, 21, 3, 18, 52, 38, 39, 41, 17, 30, 24};
    int n = (int)(sizeof(keys) / sizeof(keys[0]));
    printf("\nInserting %d keys:\n", n);
    for (int i = 0; i < n; i++) {
        insert(&H, keys[i]);
        printf("  Insert(%d)\n", keys[i]);
    }
    displayHeap("After inserts (flat root list)", &H);

    printf("\n>>> Extract-Min() twice to consolidate trees\n");
    Node *z = extractMin(&H);
    printf("Extract-Min -> %d\n", z->key);
    free(z);
    displayHeap("After 1st Extract-Min (consolidated)", &H);

    z = extractMin(&H);
    printf("Extract-Min -> %d\n", z->key);
    free(z);
    displayHeap("After 2nd Extract-Min", &H);

    printf("\n>>> Decrease-Key on a child (may mark parent)\n");
    Node *x = findKey(&H, 24);
    if (!x) x = findKey(&H, 30);
    if (x) {
        decreaseKey(&H, x, 5);
        displayHeap("After Decrease-Key", &H);
    }

    printf("\n>>> Decrease-Key on a grandchild (marks its parent)\n");
    x = findKey(&H, 39);
    if (x) {
        decreaseKey(&H, x, 2);
        displayHeap("After Decrease-Key(39 -> 2)", &H);
    }

    printf("\n>>> Decrease-Key on sibling 52 (marks node 18)\n");
    x = findKey(&H, 52);
    if (x) {
        decreaseKey(&H, x, 1);
        displayHeap("After Decrease-Key(52 -> 1)", &H);
    }

    printf("\n>>> Decrease-Key on 38 — 18 already marked, so cascading cut\n");
    x = findKey(&H, 38);
    if (x) {
        decreaseKey(&H, x, 0);
        displayHeap("After cascading cut", &H);
    }

    printf("\n>>> Delete a node (Decrease-Key to -INF + Extract-Min)\n");
    x = findKey(&H, 21);
    if (x) {
        deleteNode(&H, x);
        displayHeap("Final heap after deletion", &H);
    }

    printf("\n============================================================\n");
    printf("  NOTES\n");
    printf("============================================================\n");
    printf("1. Extract-Min consolidates the root list (link equal-degree trees).\n");
    printf("2. Decrease-Key cuts a violating child to the root list.\n");
    printf("3. A node that loses a second child is cut (cascading cut).\n");
    printf("4. Delete = Decrease-Key(-INF) followed by Extract-Min.\n");
    printf("============================================================\n");

    while (H.min) {
        z = extractMin(&H);
        free(z);
    }
    return 0;
}
