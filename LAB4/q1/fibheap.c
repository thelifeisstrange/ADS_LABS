#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int degree;
    int mark; /* unused by insert / find-min / union, kept for Fibonacci Heap structure */
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

/* Concatenate circular doubly-linked lists a and b. Either may be NULL. */
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
    x->mark = 0;
    if (!H->min) {
        x->left = x->right = x;
        H->min = x;
    } else {
        concatLists(H->min, x);
        if (x->key < H->min->key)
            H->min = x;
    }
}

void insert(FibHeap *H, int key) {
    Node *x = createNode(key);
    insertRoot(H, x);
    H->n++;
}

int findMin(FibHeap *H, int *ok) {
    if (!H->min) {
        *ok = 0;
        return 0;
    }
    *ok = 1;
    return H->min->key;
}

/* Merge H2 into H1 in O(1) by concatenating root lists. No consolidation. */
void unionHeaps(FibHeap *H1, FibHeap *H2) {
    if (!H2->min)
        return;
    if (!H1->min) {
        H1->min = H2->min;
        H1->n = H2->n;
    } else {
        concatLists(H1->min, H2->min);
        if (H2->min->key < H1->min->key)
            H1->min = H2->min;
        H1->n += H2->n;
    }
    H2->min = NULL;
    H2->n = 0;
}

void printSubtree(Node *x, int indent) {
    if (!x) return;
    Node *start = x;
    do {
        for (int i = 0; i < indent; i++) printf("  ");
        printf("- %d  (deg=%d, mark=%d)\n", start->key, start->degree, start->mark);
        if (start->child)
            printSubtree(start->child, indent + 1);
        start = start->right;
    } while (start != x);
}

void displayHeap(const char *title, FibHeap *H) {
    printf("\n--- %s ---\n", title);
    printf("n = %d\n", H->n);
    if (!H->min) {
        printf("Heap empty. Find-Min: NONE\n");
        return;
    }
    printf("Find-Min: %d\n", H->min->key);
    printf("Root list (circular, starting at min): ");
    Node *r = H->min;
    do {
        printf("%d", r->key);
        r = r->right;
        if (r != H->min) printf(" <-> ");
    } while (r != H->min);
    printf(" <-> (back)\n");
    printf("Forest (each root and its children; insert/union do not link trees):\n");
    r = H->min;
    int tree = 1;
    do {
        printf("  Tree %d:\n", tree++);
        printf("    - %d  (deg=%d, mark=%d)\n", r->key, r->degree, r->mark);
        if (r->child)
            printSubtree(r->child, 3);
        r = r->right;
    } while (r != H->min);
}

void freeSubtree(Node *x) {
    if (!x) return;
    Node *start = x;
    Node *cur = x;
    do {
        Node *next = cur->right;
        freeSubtree(cur->child);
        free(cur);
        cur = next;
    } while (cur != start);
}

void freeHeap(FibHeap *H) {
    if (H->min)
        freeSubtree(H->min);
    H->min = NULL;
    H->n = 0;
}

void printMinCheck(FibHeap *H) {
    int ok, m = findMin(H, &ok);
    if (!ok)
        printf("Verification Find-Min(): heap is empty\n");
    else
        printf("Verification Find-Min(): %d\n", m);
}

int main(void) {
    FibHeap H1, H2;
    initHeap(&H1);
    initHeap(&H2);

    printf("============================================================\n");
    printf("  LAB 4 Q1: FIBONACCI HEAP — INSERT, FIND-MIN, UNION\n");
    printf("============================================================\n");
    printf("Lazy consolidation: Insert and Union only splice root lists.\n");
    printf("Trees are not linked until a later Extract-Min (not in this lab).\n");

    displayHeap("H1 initial", &H1);
    printMinCheck(&H1);

    int h1_keys[] = {10, 5, 20, 3, 15};
    int n1 = (int)(sizeof(h1_keys) / sizeof(h1_keys[0]));
    for (int i = 0; i < n1; i++) {
        displayHeap("H1 BEFORE Insert", &H1);
        printf("\n>>> Insert(H1, %d)\n", h1_keys[i]);
        insert(&H1, h1_keys[i]);
        displayHeap("H1 AFTER Insert", &H1);
        printMinCheck(&H1);
    }

    printf("\n============================================================\n");
    printf("  Build second heap H2\n");
    printf("============================================================\n");

    int h2_keys[] = {8, 2, 12};
    int n2 = (int)(sizeof(h2_keys) / sizeof(h2_keys[0]));
    for (int i = 0; i < n2; i++) {
        displayHeap("H2 BEFORE Insert", &H2);
        printf("\n>>> Insert(H2, %d)\n", h2_keys[i]);
        insert(&H2, h2_keys[i]);
        displayHeap("H2 AFTER Insert", &H2);
        printMinCheck(&H2);
    }

    printf("\n============================================================\n");
    printf("  Union(H1, H2)  — concatenate root lists, O(1)\n");
    printf("============================================================\n");
    displayHeap("H1 BEFORE Union", &H1);
    displayHeap("H2 BEFORE Union", &H2);
    printf("\n>>> Union(H1, H2)\n");
    unionHeaps(&H1, &H2);
    displayHeap("H1 AFTER Union (merged heap)", &H1);
    displayHeap("H2 AFTER Union (emptied)", &H2);
    printMinCheck(&H1);

    printf("\n============================================================\n");
    printf("  Extra inserts into the merged heap\n");
    printf("============================================================\n");
    int extra[] = {1, 9};
    for (int i = 0; i < 2; i++) {
        displayHeap("H1 BEFORE Insert", &H1);
        printf("\n>>> Insert(H1, %d)\n", extra[i]);
        insert(&H1, extra[i]);
        displayHeap("H1 AFTER Insert", &H1);
        printMinCheck(&H1);
    }

    printf("\n============================================================\n");
    printf("  NOTES\n");
    printf("============================================================\n");
    printf("1. Insert adds a single-node tree to the root list and updates min.\n");
    printf("2. Find-Min is O(1): the min pointer is maintained on every change.\n");
    printf("3. Union concatenates the two circular root lists and keeps the\n");
    printf("   smaller of the two minima. No child linking is performed.\n");
    printf("4. The root list can grow linearly; that is lazy consolidation.\n");
    printf("============================================================\n");

    freeHeap(&H1);
    freeHeap(&H2);
    return 0;
}
