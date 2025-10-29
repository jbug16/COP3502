#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    int data;
    struct Node* next;
} Node;

struct Node* createNode(int data) {
    struct Node* head = malloc(sizeof(Node));
    head->data = data;
    return head;
}

void print(struct Node* head) {
    if (head == NULL) {
        printf("\n");
        return;
    }
    printf("%d ", head->data);
    print(head->next);
}

struct Node* del(struct Node* head, int delVal) {
    if (head == NULL) return head; // base case

    // Compare the current data to the delval
    if (head->data == delVal) {
        struct Node* temp = head->next; // store our next node
        free(head); // free the node
        return del(temp, delVal); // go to the next one and check
    }

    // Link any node together if there was a deletion
    // Also check if head->next = delval
    head->next = del(head->next, delVal);
    return head;
}

int main() {

    struct Node* list = createNode(1);
    list->next = createNode(2);
    list->next->next = createNode(2);
    list->next->next->next = createNode(3);
    list->next->next->next->next = NULL;

    print(list);

    del(list, 2);
    print(list);

    free(list);

    return 0;
}