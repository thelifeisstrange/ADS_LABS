#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *top = NULL;

void push(int n) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        printf("Stack Overflow (malloc failed)\n");
        return;
    }
    node->data = n;
    node->next = top;
    top = node;
    printf("successfully pushed %d in the stack\n", n);
}

void pop(void) {
    if (!top) {
        printf("Stack Underflow\n");
        return;
    }
    Node *temp = top;
    printf("%d popped from the stack\n", temp->data);
    top = top->next;
    free(temp);
}

void multipop(int n) {
    for (int i = 0; i < n; i++) {
        if (!top) {
            printf("Stack Underflow\n");
            return;
        }
        pop();
    }
}

void peek(void) {
    if (!top) {
        printf("Stack is empty\n");
        return;
    }
    printf("Top of stack is %d\n", top->data);
}

void display(void) {
    if (!top) {
        printf("Stack is empty\n");
        return;
    }
    printf("Stack Elements (top to bottom): ");
    for (Node *p = top; p; p = p->next)
        printf("%d ", p->data);
    printf("\n");
}

int main(void) {
    int choice, value;

    while (1) {
        printf("Stack Operations (Linked List)\n");
        printf("Choose Options\n");
        printf("[1]Insert Element\n");
        printf("[2]Pop Element\n");
        printf("[3]MultiPop Elements\n");
        printf("[4]Peek\n");
        printf("[5]Display\n");
        printf("[6]Exit\n");
        if (scanf("%d", &choice) != 1)
            break;

        switch (choice) {
            case 1:
                printf("Enter value to be inserted in stack\n");
                scanf("%d", &value);
                push(value);
                break;
            case 2:
                pop();
                break;
            case 3:
                printf("Enter number of values to be popped\n");
                scanf("%d", &value);
                multipop(value);
                break;
            case 4:
                peek();
                break;
            case 5:
                display();
                break;
            case 6:
                printf("Exiting Program\n");
                while (top) pop();
                return 0;
            default:
                printf("Invalid Choice! Please try again\n");
        }
    }
    return 0;
}
