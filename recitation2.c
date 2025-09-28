//
// Created by Jenna Curls on 9/11/25.
//

#include <stdio.h>

int zeros(int n) {
    int res = 0;
    if (n != 0) {
        n /= 5;
        res += n;
        zeros(n);
    }
    return res;
}

int sumEven(int vals[], int low, int high) {
    if (low <= high) {
        if (vals[low] % 2 == 0) {
            return vals[low] + sumEven(vals, low+1, high);
        }
        else return sumEven(vals, low+1, high);
    }
    else return 0;
}

int main() {
    //printf("%d", zeros(100));
    int vals[] = {15, 13, 28, 19, 24, 8, 7, 99, 10, 14};
    printf("%d", sumEven(vals, 3, 8));
    return 0;
}