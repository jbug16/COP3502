//
// Created by Jenna Curls on 11/24/25.
//

#include <stdio.h>
#include <stdlib.h>

// Prompt was to create a function that adds the digits of numbers in a list
// then adds the total of the list. The digits can only be added 2 times per
// number in the list.
// Ex: [3, 29, 15] -> [3, 11, 15] -> [3, 2, 6] -> 11
int addDigits(int* list, int n) {
    int total = 0;

    // Loop through each index in list
    for (int i = 0; i < n; i++) {
        int sum = 0;
        int num = list[i];

        // Only let us add these digits twice
        for (int j = 0; j < 2; j++) {
            // Add these digits
            while (num != 0) {
                int r = num % 10;
                sum += r;
                num /= 10;
            }

            // Reset values
            list[i] = sum;
            num = list[i];
            sum = 0;

            // If we are a single digit, do not try to add again
            if (list[i] < 10) break;
        }

        // Add to the total sum (each list index)
        total += list[i];
    }

    return total;
}

int main() {
    int size = 3;
    int list[] = {199, 199, 199};

    printf("%d\n", addDigits(list, size));

    return 0;
}