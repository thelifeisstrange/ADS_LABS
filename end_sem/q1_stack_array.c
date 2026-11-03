#include <stdio.h>
#define MAX 100

int stack[MAX], top = -1;

void push(int x) {
    if (top == MAX - 1) { printf("Stack Overflow\n"); return; }
    stack[++top] = x;
}

void pop() {
    if (top == -1) { printf("Stack Underflow\n"); return; }
    printf("Popped: %d\n", stack[top--]);
}

void display() {
    if (top == -1) { printf("Stack is empty\n"); return; }
    printf("Stack (top to bottom): ");
    for (int i = top; i >= 0; i--) printf("%d ", stack[i]);
    printf("\n");
}

int main() {
    int choice, x;
    while (1) {
        printf("\n1.Push  2.Pop  3.Display  4.Exit : ");
        scanf("%d", &choice);
        if (choice == 1)      { printf("Value: "); scanf("%d", &x); push(x); }
        else if (choice == 2) pop();
        else if (choice == 3) display();
        else break;
    }
    return 0;
}
