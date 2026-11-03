#include <stdio.h>
#define MAX 100

int stack[MAX], top = -1;
int ops = 0, totalCost = 0;

// log each operation: its actual cost, running total, running average
void record(const char *name, int cost) {
    ops++;
    totalCost += cost;
    printf("%-4d %-9s cost=%-3d total=%-4d avg=%.2f\n",
           ops, name, cost, totalCost, (float)totalCost / ops);
}

void push(int x)    { if (top < MAX - 1) { stack[++top] = x; record("push", 1); } }
void pop()          { if (top >= 0)      { top--;             record("pop", 1);  } }
void multipop(int k) {
    int cost = 0;
    while (top >= 0 && k-- > 0) { top--; cost++; }
    record("multipop", cost);
}

int main() {
    for (int i = 1; i <= 10; i++) push(i);   // 10 pushes
    pop(); pop();                            // 2 pops
    multipop(5);                             // pops 5
    for (int i = 0; i < 4; i++) push(i);     // 4 pushes
    multipop(10);                            // pops the rest

    printf("\nTotal operations n = %d\n", ops);
    printf("Total actual cost  = %d\n", totalCost);
    printf("Average cost       = %.2f\n", (float)totalCost / ops);
    printf("\nAggregate analysis: every element is popped at most once after it is pushed,\n");
    printf("so total pop cost <= total pushes <= n. Total cost <= 2n,\n");
    printf("amortized cost per operation <= 2n / n = O(1).\n");
    return 0;
}
