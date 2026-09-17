#include <stdio.h>

#define MAX 100

typedef struct {
    int data[MAX];
    int top;
} Stack;

void init(Stack *s) { s->top = -1; }

int push(Stack *s, int val) {
    if (s->top >= MAX - 1)
        return 0;
    s->data[++s->top] = val;
    return 1; /* actual cost of PUSH is 1 */
}

int pop(Stack *s, int *val) {
    if (s->top < 0)
        return 0;
    *val = s->data[s->top--];
    return 1; /* actual cost of POP is 1 */
}

int multipop(Stack *s, int k) {
    int cost = 0;
    while (k > 0 && s->top >= 0) {
        s->top--;
        k--;
        cost++;
    }
    return cost; /* actual cost = number of pops performed */
}

int main(void) {
    Stack s;
    init(&s);

    /* 1 = PUSH, 2 = POP, 3 = MULTIPOP */
    int op_types[] = {1, 1, 1, 1, 1, 1, 3, 1, 1, 2, 3, 1};
    int op_vals[] = {10, 20, 30, 40, 50, 60, 4, 70, 80, 0, 3, 90};
    int n = (int)(sizeof(op_types) / sizeof(op_types[0]));
    int total = 0;

    printf("======================================================================\n");
    printf("  LAB SET 2 EXP 1: STACK (ARRAY) — AGGREGATE ANALYSIS\n");
    printf("======================================================================\n");
    printf("%-4s | %-10s | %-10s | %-6s | %-12s | %-10s\n",
           "Op#", "Operation", "Arg", "Size", "Actual cost", "Total cost");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        int cost = 0;
        char name[12], arg[12];

        if (op_types[i] == 1) {
            snprintf(name, sizeof(name), "PUSH");
            snprintf(arg, sizeof(arg), "%d", op_vals[i]);
            cost = push(&s, op_vals[i]);
        } else if (op_types[i] == 2) {
            int v = 0;
            cost = pop(&s, &v);
            snprintf(name, sizeof(name), "POP");
            snprintf(arg, sizeof(arg), "%d", v);
        } else {
            int k = op_vals[i];
            cost = multipop(&s, k);
            snprintf(name, sizeof(name), "MULTIPOP");
            snprintf(arg, sizeof(arg), "k=%d", k);
        }

        total += cost;
        printf("%-4d | %-10s | %-10s | %-6d | %-12d | %-10d\n",
               i + 1, name, arg, s.top + 1, cost, total);
    }

    printf("======================================================================\n\n");
    printf("AGGREGATE ANALYSIS\n");
    printf("  n  = %d operations\n", n);
    printf("  T(n) = sum of actual costs = %d\n", total);
    printf("  Average cost = T(n)/n = %d/%d = %.4f\n\n", total, n, (double)total / n);
    printf("Proof that amortized cost is O(1):\n");
    printf("  1. Each PUSH costs 1 and each element is pushed at most once.\n");
    printf("  2. Each POP / item removed by MULTIPOP costs 1, and an element\n");
    printf("     can be popped at most once after it is pushed.\n");
    printf("  3. Therefore T(n) <= 2n  (at most n pushes and n pops).\n");
    printf("  4. Amortized cost per operation = T(n)/n <= 2 = O(1).\n");
    printf("     MULTIPOP(k) may cost up to k in the worst case, but that\n");
    printf("     cost was already paid for by earlier PUSH operations.\n");
    printf("======================================================================\n");
    return 0;
}
