//
// Created by Jenna Curls on 9/25/25.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // Set up stack
    struct stack* myStackPtr;
    init(&myStackPtr);

    char next;
    while (scanf(" %c", &next) == 1) {

        // Push the char onto the stack
        if (isalpha(next)) {
            int index = toupper(next) - 'A';
            int nextInt = convertToBinary(values[index]);
            push(&myStackPtr, nextInt);
        }
        // Not operator
        else if (next == '-') {
            struct stack* temp = pop(&myStackPtr);
            int val1 = temp->data;
            free(temp);

            // Evaluate with operator
            int result = 0;
            result = !val1;
            printf("!%d = %d\n", val1, result);

            push(&myStackPtr, result);
        }
        // Got an operator
        else {
            struct stack* temp = pop(&myStackPtr);
            int val2 = temp->data;
            free(temp);
            temp = pop(&myStackPtr);
            int val1 = temp->data;
            free(temp);

            // Evaluate with operator
            int result = -1;
            if (next == '*') result = val1 && val2;
            else if (next == '+') result = val1 || val2;
            printf("%d %c %d = %d\n", val1, next, val2, result);

            push(&myStackPtr, result);
        }
    }

    struct stack* temp = pop(&myStackPtr);
    printf("%c\n", convertToText(temp->data));

    // Clean up
    free(values);

    return 0;
}

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

int convertToBinary(char value) {
    return value == 'T' ? 1 : 0;
}

char convertToText(int value) {
    return value ? 'T' : 'F';
}