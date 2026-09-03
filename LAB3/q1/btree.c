#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int t;

typedef struct Node {
    int *keys, n;
    struct Node **C;
    bool leaf;
} Node;

Node* createNode(bool leaf) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->leaf = leaf;
    node->keys = (int*)malloc((2 * t - 1) * sizeof(int));
    node->C = (Node**)malloc(2 * t * sizeof(Node*));
    node->n = 0;
    return node;
}

void printTree(Node* root) {
    if (!root) { printf("Tree Empty\n"); return; }
    Node* q[100]; int lvl[100], f = 0, r = 0, cur = -1;
    q[r] = root; lvl[r++] = 0;
    while (f < r) {
        Node* u = q[f]; int l = lvl[f++];
        if (l != cur) { cur = l; printf("\n  [L%d]: ", l); }
        printf("[");
        for (int i = 0; i < u->n; i++) printf("%d%s", u->keys[i], i == u->n - 1 ? "" : " ");
        printf("] ");
        if (!u->leaf)
            for (int i = 0; i <= u->n; i++) { q[r] = u->C[i]; lvl[r++] = l + 1; }
    }
    printf("\n");
}

void traverse(Node* r) {
    if (!r) return;
    int i;
    for (i = 0; i < r->n; i++) {
        if (!r->leaf) traverse(r->C[i]);
        printf("%d ", r->keys[i]);
    }
    if (!r->leaf) traverse(r->C[i]);
}

Node* search(Node* r, int k) {
    int i = 0;
    while (r && i < r->n && k > r->keys[i]) i++;
    if (r && i < r->n && r->keys[i] == k) return r;
    return (!r || r->leaf) ? NULL : search(r->C[i], k);
}

void splitChild(Node* x, int i, Node* y) {
    Node* z = createNode(y->leaf);
    z->n = t - 1;
    for (int j = 0; j < t - 1; j++) z->keys[j] = y->keys[j + t];
    if (!y->leaf)
        for (int j = 0; j < t; j++) z->C[j] = y->C[j + t];
    y->n = t - 1;
    for (int j = x->n; j >= i + 1; j--) x->C[j + 1] = x->C[j];
    x->C[i + 1] = z;
    for (int j = x->n - 1; j >= i; j--) x->keys[j + 1] = x->keys[j];
    x->keys[i] = y->keys[t - 1];
    x->n++;
}

void insertNonFull(Node* x, int k) {
    int i = x->n - 1;
    if (x->leaf) {
        while (i >= 0 && x->keys[i] > k) { x->keys[i + 1] = x->keys[i]; i--; }
        x->keys[i + 1] = k;
        x->n++;
    } else {
        while (i >= 0 && x->keys[i] > k) i--;
        if (x->C[++i]->n == 2 * t - 1) {
            splitChild(x, i, x->C[i]);
            if (x->keys[i] < k) i++;
        }
        insertNonFull(x->C[i], k);
    }
}

Node* insert(Node* root, int k) {
    if (!root) { root = createNode(true); root->keys[0] = k; root->n = 1; return root; }
    if (root->n == 2 * t - 1) {
        Node* s = createNode(false);
        s->C[0] = root;
        splitChild(s, 0, root);
        insertNonFull(s, k);
        return s;
    }
    insertNonFull(root, k);
    return root;
}

void merge(Node* x, int i) {
    Node *y = x->C[i], *z = x->C[i + 1];
    y->keys[t - 1] = x->keys[i];
    for (int j = 0; j < z->n; j++) y->keys[j + t] = z->keys[j];
    if (!y->leaf)
        for (int j = 0; j <= z->n; j++) y->C[j + t] = z->C[j];
    for (int j = i + 1; j < x->n; j++) x->keys[j - 1] = x->keys[j];
    for (int j = i + 2; j <= x->n; j++) x->C[j - 1] = x->C[j];
    y->n += z->n + 1; x->n--;
    free(z);
}

void removeKey(Node* x, int k) {
    int i = 0;
    while (i < x->n && x->keys[i] < k) i++;
    if (i < x->n && x->keys[i] == k) {
        if (x->leaf) {
            for (int j = i + 1; j < x->n; j++) x->keys[j - 1] = x->keys[j];
            x->n--;
        } else {
            Node* cur = x->C[i];
            while (!cur->leaf) cur = cur->C[cur->n];
            int pred = cur->keys[cur->n - 1];
            x->keys[i] = pred;
            removeKey(x->C[i], pred);
        }
    } else if (!x->leaf) {
        if (x->C[i]->n < t) {
            if (i > 0 && x->C[i - 1]->n >= t) {
                Node *c = x->C[i], *s = x->C[i - 1];
                for (int j = c->n - 1; j >= 0; j--) c->keys[j + 1] = c->keys[j];
                c->keys[0] = x->keys[i - 1];
                x->keys[i - 1] = s->keys[--s->n];
                c->n++;
            } else if (i < x->n && x->C[i + 1]->n >= t) {
                Node *c = x->C[i], *s = x->C[i + 1];
                c->keys[c->n++] = x->keys[i];
                x->keys[i] = s->keys[0];
                for (int j = 1; j < s->n; j++) s->keys[j - 1] = s->keys[j];
                s->n--;
            } else merge(x, i == x->n ? i - 1 : i);
        }
        removeKey(x->C[i], k);
    }
}

Node* deleteRoot(Node* root, int k) {
    if (!root) return NULL;
    removeKey(root, k);
    if (root->n == 0) {
        Node* tmp = root;
        root = root->leaf ? NULL : root->C[0];
        free(tmp);
    }
    return root;
}

int main() {
    printf("Enter minimum degree 't': ");
    if (scanf("%d", &t) != 1 || t < 2) t = 3;

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = sizeof(keys) / sizeof(keys[0]);
    Node* root = NULL;

    printf("\n--- INSERTION ---");
    for (int i = 0; i < n; i++) {
        printf("\nInsert(%d):", keys[i]);
        root = insert(root, keys[i]);
        printTree(root);
    }

    printf("\n--- SEARCH (6) ---\nResult: %s\n", search(root, 6) ? "FOUND" : "NOT FOUND");

    printf("\n--- DELETE (6) ---");
    root = deleteRoot(root, 6);
    printTree(root);

    printf("\nTraversal: ");
    traverse(root);
    printf("\n");
    return 0;
}
