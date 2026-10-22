#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} DynamicArray;

void initArray(DynamicArray *arr) {
    arr->capacity = 1;
    arr->size = 0;
    arr->data = (int *)malloc(arr->capacity * sizeof(int));
}

int push(DynamicArray *arr, int val, int *doubled_flag, int *copied_count) {
    int cost = 1;
    *doubled_flag = 0;
    *copied_count = 0;

    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        int *new_data = (int *)malloc(arr->capacity * sizeof(int));
        for (int i = 0; i < arr->size; i++)
            new_data[i] = arr->data[i];
        free(arr->data);
        arr->data = new_data;

        *copied_count = arr->size;
        cost += arr->size;
        *doubled_flag = 1;
    }

    arr->data[arr->size++] = val;
    return cost;
}

int pop(DynamicArray *arr, int *popped_val) {
    if (arr->size == 0) {
        *popped_val = -1;
        return 0;
    }
    *popped_val = arr->data[--arr->size];
    return 1;
}

int multipop(DynamicArray *arr, int k, int *actual_popped) {
    *actual_popped = (k > arr->size) ? arr->size : k;
    arr->size -= *actual_popped;
    return *actual_popped;
}

void freeArray(DynamicArray *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

int main(void) {
    DynamicArray arr;
    initArray(&arr);

    int op_types[] = {1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 2, 1};
    int op_values[] = {10, 20, 30, 40, 50, 60, 70, 80, 4, 90, 100, 8, 0, 110};
    int num_ops = (int)(sizeof(op_types) / sizeof(op_types[0]));
    int total_cost = 0;

    printf("====================================================================================================================\n");
    printf("                     LAB 2: DYNAMIC TABLE — AGGREGATE ANALYSIS (WITH MULTIPOP)                                      \n");
    printf("====================================================================================================================\n");
    printf("%-4s | %-12s | %-10s | %-6s | %-10s | %-16s | %-12s | %-10s\n",
           "Op#", "Operation", "Param/Val", "Size", "Capacity", "Resize Occur", "Actual Cost", "Total Cost");
    printf("--------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < num_ops; i++) {
        int cost = 0;
        int doubled = 0;
        int copied = 0;
        char op_name[14];
        char val_str[14];

        if (op_types[i] == 1) {
            snprintf(op_name, sizeof(op_name), "PUSH");
            snprintf(val_str, sizeof(val_str), "%d", op_values[i]);
            cost = push(&arr, op_values[i], &doubled, &copied);
        } else if (op_types[i] == 2) {
            int popped_val;
            snprintf(op_name, sizeof(op_name), "POP");
            cost = pop(&arr, &popped_val);
            if (popped_val != -1)
                snprintf(val_str, sizeof(val_str), "%d", popped_val);
            else
                snprintf(val_str, sizeof(val_str), "EMPTY");
        } else if (op_types[i] == 3) {
            int actual_popped = 0;
            int req_k = op_values[i];
            cost = multipop(&arr, req_k, &actual_popped);
            snprintf(op_name, sizeof(op_name), "MULTIPOP");
            snprintf(val_str, sizeof(val_str), "k=%d (pop %d)", req_k, actual_popped);
        }

        total_cost += cost;

        char resize_str[24];
        if (doubled)
            snprintf(resize_str, sizeof(resize_str), "YES (copied %d)", copied);
        else
            snprintf(resize_str, sizeof(resize_str), "NO");

        printf("%-4d | %-12s | %-10s | %-6d | %-10d | %-16s | %-12d | %-10d\n",
               i + 1, op_name, val_str, arr.size, arr.capacity, resize_str, cost, total_cost);
    }

    printf("====================================================================================================================\n\n");

    double amortized_cost = (double)total_cost / num_ops;
    printf("Total Operations (n)       : %d\n", num_ops);
    printf("Total Actual Cost (Sum C_i): %d\n", total_cost);
    printf("Average / Amortized Cost   : %d / %d = %.4f ~ O(1)\n", total_cost, num_ops, amortized_cost);

    freeArray(&arr);
    return 0;
}
