#include <stdio.h>
#include <stdlib.h>

#define NUM_BITS 6
#define NUM_OPS 16

typedef struct {
    int bits[NUM_BITS];       // Bit array (bits[0] is LSB)
    int credit[NUM_BITS];     // Credit stored on each individual bit
} BinaryCounter;

void initCounter(BinaryCounter *bc) {
    for (int i = 0; i < NUM_BITS; i++) {
        bc->bits[i] = 0;
        bc->credit[i] = 0;
    }
}

// Function to print current counter in binary (MSB to LSB)
void printBinary(BinaryCounter *bc) {
    for (int i = NUM_BITS - 1; i >= 0; i--) {
        printf("%d", bc->bits[i]);
    }
}

// Increment operation tracking actual cost, amortized cost, and credit
void increment(BinaryCounter *bc, int *actual_cost, int *amortized_cost, int *flips_0_to_1, int *flips_1_to_0) {
    int i = 0;
    *actual_cost = 0;
    *flips_0_to_1 = 0;
    *flips_1_to_0 = 0;

    // Flip trailing 1s to 0s (paid by stored credit)
    while (i < NUM_BITS && bc->bits[i] == 1) {
        bc->bits[i] = 0;
        bc->credit[i] = 0; // Credit consumed to pay for flipping to 0
        (*flips_1_to_0)++;
        (*actual_cost)++;
        i++;
    }

    // Flip the first 0 to 1 (if within bounds)
    if (i < NUM_BITS) {
        bc->bits[i] = 1;
        bc->credit[i] = 1; // 1 credit stored on this bit for future reset
        (*flips_0_to_1)++;
        (*actual_cost)++;
        *amortized_cost = 2; // Charged: 1 for flip + 1 stored as credit
    } else {
        // Counter overflow
        *amortized_cost = 0;
    }
}

int getTotalCredit(BinaryCounter *bc) {
    int sum = 0;
    for (int i = 0; i < NUM_BITS; i++) {
        sum += bc->credit[i];
    }
    return sum;
}

int main() {
    BinaryCounter bc;
    initCounter(&bc);

    int total_actual_cost = 0;
    int total_amortized_cost = 0;

    printf("====================================================================================================================\n");
    printf("                                EXPERIMENT 2: ACCOUNTING METHOD (BINARY COUNTER)                                     \n");
    printf("====================================================================================================================\n");
    printf("%-4s | %-10s | %-12s | %-11s | %-11s | %-14s | %-13s\n",
           "Op#", "Counter", "1->0 Flips", "0->1 Flips", "Actual(c_i)", "Amortized(c^)", "Bank Credit");
    printf("--------------------------------------------------------------------------------------------------------------------\n");

    // Initial State
    printf("%-4s | ", "Init");
    printBinary(&bc);
    printf("   | %-12s | %-11s | %-11s | %-14s | %-13d\n", "-", "-", "-", "-", getTotalCredit(&bc));

    for (int op = 1; op <= NUM_OPS; op++) {
        int actual_cost = 0;
        int amortized_cost = 0;
        int flips_0_to_1 = 0;
        int flips_1_to_0 = 0;

        increment(&bc, &actual_cost, &amortized_cost, &flips_0_to_1, &flips_1_to_0);

        total_actual_cost += actual_cost;
        total_amortized_cost += amortized_cost;
        int bank_credit = getTotalCredit(&bc);

        printf("%-4d | ", op);
        printBinary(&bc);
        printf("   | %-12d | %-11d | %-11d | %-14d | %-13d\n",
               flips_1_to_0, flips_0_to_1, actual_cost, amortized_cost, bank_credit);
    }

    printf("====================================================================================================================\n\n");

    printf("============================================= MATHEMATICAL ANALYSIS =============================================\n");
    printf("Total Operations (n)                : %d\n", NUM_OPS);
    printf("Total Actual Cost (Sum c_i)         : %d\n", total_actual_cost);
    printf("Total Amortized Cost (Sum c^_i)     : %d\n", total_amortized_cost);
    printf("Remaining Credit in Bank            : %d\n\n", getTotalCredit(&bc));

    printf("THEORETICAL VERIFICATION:\n");
    printf("1. Validity Condition: Sum(c^_i) >= Sum(c_i) for all n.\n");
    printf("   - Here: %d >= %d (Credit Bank is always non-negative: %d >= 0).\n",
           total_amortized_cost, total_actual_cost, getTotalCredit(&bc));
    printf("2. Amortized Cost Per Operation:\n");
    printf("   - Every increment sets at most ONE '0' to '1' -> Allocated charge = 2 units.\n");
    printf("   - Resetting any number of '1's to '0's consumes stored credit (Amortized charge = 0).\n");
    printf("   - Therefore, Amortized Cost per increment = O(1) [constant = 2].\n");
    printf("=================================================================================================================\n");

    return 0;
}
