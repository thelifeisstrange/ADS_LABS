#include<stdio.h>
#include<stdlib.h>


#define MAX 1000

int stack[MAX];
int top = -1;

void push(int n){
    if (top >= MAX - 1) {
    printf("Stack Overflow\n");
    } else {
        stack[++top] = n;
        printf("successfully pushed %d in the stack\n", n);
    }
}

void pop(){
    if (top == -1) {
        printf("Stack Underflow\n");
    }
    else{
        printf("%d popped from the stack", stack[top]);
        top -= 1;
    }
}

void multipop(int n){
    for(int i = 0; i < n; i++){
        if(stack[top]<=-1){
            printf("Stack Underflow\n");
        }else{
            printf("%d popped from the stack", stack[top]);
            top -= 1;
        }
    }
}

void peek(){
    if (top == -1) {
    printf("Stack is empty\n");
} else {
    printf("Top of stack is %d\n", stack[top]);
}
}

void display(){
    if (top == -1) {
    printf("Stack is empty\n");}
    else{
    printf("Stack Elements: ");
    for (int i = 0; i <= top; i++) {
    printf("%d ", stack[i]);
    }
    printf("\n");
    }
}

int main(){
    
    int choice, value;

    while(1){
        printf("Stack Operations\n");
        printf("Choose Options\n");
        printf("[1]Insert Element\n");
        printf("[2]Pop Element\n");
        printf("[3]MultiPop Elements\n");
        printf("[4]Peek\n");
        printf("[5]Display\n");
        printf("[6]Exit\n");
        scanf("%d", &choice);

        switch(choice){
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
                exit(0);
            
            default:
                printf("Invalid Choice! Please try again\n");

        }
    }
}