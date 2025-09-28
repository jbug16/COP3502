//
// Created by Jenna Curls on 8/25/25.
//

/* 1) Write a program that has a few functions that perform the following tasks:
a) A function that takes in an integer n, representing the number of items, and an integer,
max, representing the maximum possible item, dynamically allocates an integer array of
size n where each number is randomly selected in between 1 and max, inclusive, and
returns a pointer to this newly created array.
b) A function that takes in an array and its size, and prints out the contents of the array.
c) A function that takes in an array and its size, and returns the maximum value in the
array.
d) A function that takes in an array and its size, and returns the average of the values in
the array.
Have your program call the function to generate the array, and then, with this array, print it, its
maximum value and its average. Feel free to add other features to the program. At the end of the
program, free the memory for the array. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* createArrayPtr(int n, int max);

int main() {
    int* arr = createArrayPtr(4, 2);
    for (int i = 0; i < 4; i++) {
        printf("%d\n", arr[i]);
    }

    free(arr);

    return 0;
}

int* createArrayPtr(int n, int max) {
    // Dynamically allocate an int array of size n
    int* arr = malloc(sizeof(n));

    for (int i = 0; i < n; i++) {
        arr[i] = rand() % (max+1);
    }

    return arr;
}