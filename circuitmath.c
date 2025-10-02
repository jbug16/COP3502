//
// Created by Jenna Curls on 9/25/25.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 1000

struct stack {
    int data;
    struct stack *next;
};

// Function definitions
void init(struct stack **front);
int push(struct stack **front, int num);
struct stack* pop(struct stack **front);
int empty(struct stack *front);
int top(struct stack *front);
int convertToBinary(char value);
char convertToText(int value);

int main() {
    // Get input for how many variables
    int n;
    scanf("%d", &n);

    // Get true of false for each variable
    char* values = malloc((n+1) * sizeof(char));
    for (int i = 0; i < n; i++) {
        scanf(" %c", &values[i]);
    }
    values[n] = '\0';

    // Set up empty stack
    struct stack* myStackPtr;
    init(&myStackPtr);

    // Read in equation input
    char equation[MAX_STR_LEN] = {0};
    scanf(" %[^\n]", equation);

    printf("%s\n", equation);

    // Go through each item
    //char next;

    //struct stack* temp = pop(&myStackPtr);
    //printf("The value is: %c\n", convertToText(temp->data));

    // Clean up
    free(values);

    return 0;
}

// Following imported from class notes
void init(struct stack **front) {
    *front = NULL;
}

int push(struct stack **front, int num) {
    struct stack* temp = malloc(sizeof(struct stack));

    if (temp != NULL) {
        temp->data = num;
        temp->next = *front;
        *front = temp;
        return 1;
    }

    return 0;
}

struct stack* pop(struct stack **front) {
    struct stack* temp;
    temp = NULL;

    if (*front != NULL) {
        temp = (*front);
        *front = (*front)->next;
        temp->next = NULL;
    }
    return temp;
}

int empty(struct stack *front) {
    return front == NULL ? 1 : 0;
}

int top(struct stack *front) {
    return front != NULL ? front->data : -1;
}

// Custom functions
int convertToBinary(char value) {
    return value == 'T' ? 1 : 0;
}

char convertToText(int value) {
    return value ? 'T' : 'F';
}