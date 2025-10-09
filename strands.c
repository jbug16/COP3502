//
// Created by Jenna Curls on 10/7/2025.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct dllnode {
    char ch;
    struct dllnode* prev;
    struct dllnode* next;
} dllnode;

// Create a node and set the value and init next/prev nodes
dllnode* createNode(char c) {
    dllnode* node = malloc(sizeof(dllnode));
    node->ch = c; // set character to c
    node->next = NULL;
    node->prev = NULL;

    return node;
}

void setNextDirection(dllnode*** strandsGrid, int dir, int row, int col) {
    // NW = 0, N = 1, NE = 2, W = 3, E = 4, SW = 5, S = 6, SE = 7
    const int NUMDIR = 8;
    const int NULLPTR = -1;
    const int DR[NUMDIR] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int DC[NUMDIR] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Get our current node (this is the current letter we are looking at)
    dllnode* current = strandsGrid[row][col];

    if (dir == NULLPTR) {
        // This means this letter/node is the LAST letter in the word
        current->next = NULL;
        return;
    }

    // Find the next row and col based on the direction input
    int nextRow = row + DR[dir];
    int nextCol = col + DC[dir];

    // Find the node of that position
    dllnode* next = strandsGrid[nextRow][nextCol];
    if (!next) return; // safety check to make sure it exists

    // Set the next node of our current node (creates the linked list part, next function will make it a dll)
    current->next = next;
}

void setPrevDirection(dllnode*** strandsGrid, int dir, int row, int col) {
    // NW = 0, N = 1, NE = 2, W = 3, E = 4, SW = 5, S = 6, SE = 7
    const int NUMDIR = 8;
    const int NULLPTR = -1;
    const int DR[NUMDIR] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int DC[NUMDIR] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Get our current node (this is the current letter we are looking at)
    dllnode* current = strandsGrid[row][col];

    if (dir == NULLPTR) {
        // This means this letter/node is the FIRST letter in the word
        current->prev = NULL;
        return;
    }

    // Find the prev row and col based on the direction input
    int prevRow = row + DR[dir];
    int prevCol = col + DC[dir];

    // Find the node of that position
    dllnode* prev = strandsGrid[prevRow][prevCol];
    if (!prev) return; // safety check to make sure it exists

    // Set the prev node of our current node (now it's a doubly linked list)
    current->prev = prev;
}

dllnode* getHead(dllnode* current) {
    // Check to see if this node (letter) is the head (start of the word)
    while (current->prev) { // basically while the prev node is not null
        current = current->prev; // we set the new node (head) to the prev one until we cant anymore
    }

    return current;
}

dllnode* getTail(dllnode* current) {
    // Check to see if this node (letter) is the tail (end of the word)
    while (current->next) { // basically while the next node is not null
        current = current->next; // we set the new node (tail) to the next one until we cant anymore
    }

    return current;
}

void printWordFromPos(dllnode*** strandsGrid, int y, int x) {
    // Get the head from the current letters position
    dllnode* head = getHead(strandsGrid[y][x]);

    // Print (using print function from doublelinklist.c notes)
    while (head != NULL) {
        printf("%c", head->ch);
        head = head->next;
    }
    printf("\n");
}

void reverseWord(dllnode*** strandsGrid, int y, int x) {
    // Get our current, head and tail node
    dllnode* current = strandsGrid[y][x];
    dllnode* head = getHead(current);
    dllnode* tail = getTail(current);

    // Now set current to the head of the list
    current = head;

    // Loop until our current node is null
    while (current != NULL) {
        dllnode* tempHead = current->next; // temp store what will be the new head (next node after current head)
        current->next = current->prev; // swap the pointers so its like we are skipping over current and pointing next to its prev
        current->prev = tempHead; // swap the pointers again
        current = tempHead; // now place the new head in the front spot
        // that was rough i spent like 2 hours rearranging spice bottles to simulate this NOW FINALLY IT WORKS my mind is numb
    }

    // Swap the head and tail values
    dllnode* tempTail = tail;
    dllnode* tempHead = head; // our original head
    head = tempTail; // say the head is now our old tail
    tail = tempHead; // say the tail is now our old head
}

void mergeWords(dllnode*** strandsGrid, int y1, int x1, int y2, int x2) {
    // This one's easy: just link the next pointer of the last node of the first word (this pointer will be
    // originally NULL) to point to the first node of the second word, and set the previous pointer for the
    // first node in the second word to point to the last node in the first word.
    // right right yeah ok

    // Get needed head and tail
    dllnode* tail1 = getTail(strandsGrid[y1][x1]);
    dllnode* head2 = getHead(strandsGrid[y2][x2]);

    // Link the end of word1 to the start of word2
    tail1->next = head2;
    head2->prev = tail1;
}

int main() {
    // 1. Print out the word from that grid position
    // 2. Print out the reverse of that word from the grid position
    // 3. If the current position is the end of a word AND an adjacent position is a starting letter, merge those words together
    
    int numRows;
    int numCols;

    // Get input for rows and cols (ex: "8 6")
    scanf("%d %d", &numRows, &numCols);

    dllnode*** strandsGrid = malloc(sizeof(dllnode**) * numRows);
    for (int i = 0; i < numRows; i++) {
        strandsGrid[i] = malloc(sizeof(dllnode*) * numCols);
    }

    // Get input for letters (ex: 8 lines of input, 6 characters each line, no space)
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            char c;
            scanf(" %c", &c);

            // Create a node for each point on the table and set the character we inputted
            strandsGrid[i][j] = createNode(c);
        }
    }

    // Get input for direction of each letter's next letter (if -1, this is the LAST letter in a word)
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            int nextDir;
            scanf("%d", &nextDir);
            if (isspace(nextDir)) break;

            // use direction number to determine what node to point to next
            setNextDirection(strandsGrid, nextDir, i, j);
        }
    }

    // Get input for direction for previous letter (if -1, this is the FIRST letter in a word)
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            int prevDir;
            scanf("%d", &prevDir);
            if (isspace(prevDir)) break;

            // use direction number to determine what node to point to prev
            setPrevDirection(strandsGrid, prevDir, i, j);
        }
    }

    // Get input for how many queries to expect
        // 1. Print the word that the node in a particular grid square is part of.
        // 2. Reverse the word that the node in a particular grid square is part of.
        // 3. Given the position of the last letter in one word, and an adjacent position that is the starting letter
        // in a different word, concatenate the word in the first position to the word in the second listed position.
    int numOfQueries;
    scanf("%d", &numOfQueries);

    // Get input (for each query) of what function to call & arguments
    for (int i = 0; i < numOfQueries; i++) {
        int queryNum;
        scanf("%d", &queryNum);
        if (queryNum == 1 || queryNum == 2) {
            int input1, input2;
            scanf(" %d %d", &input1, &input2);

            if (queryNum == 1) printWordFromPos(strandsGrid, input1, input2);
            else reverseWord(strandsGrid, input1, input2);
        }
        else if (queryNum == 3) {
            int input1, input2, input3, input4;
            scanf(" %d %d %d %d", &input1, &input2, &input3, &input4);
            mergeWords(strandsGrid, input1, input2, input3, input4);
        }
    }

    // (Output for query 1 only) Print the word that grid position is apart of
    // This is done in the query 1 function (hopefully that's ok because i cant figure out how else to do it ahhaha :)) )

    // Cleanup
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            free(strandsGrid[i][j]);
        }
        free(strandsGrid[i]);
    }
    free(strandsGrid);

    return 0;
}

/*

8 6
cexgre
omaniu
lledet
rqgarc
euecoe
pmiaul
aoewrs
phkroe
6 4 6 -1 4 -1
6 -1 3 1 3 0
4 4 6 2 5 1
-1 6 6 1 1 1
1 7 -1 4 6 1
1 7 4 -1 6 1
1 1 4 7 4 6
1 1 -1 3 3 -1
-1 -1 3 6 7 3
1 4 1 4 5 6
1 3 3 6 6 6
6 -1 1 2 -1 6
6 1 1 -1 3 6
6 6 0 3 1 -1
6 6 0 3 1 3
-1 -1 4 4 0 1
10
1 1 0
1 6 2
1 5 4
2 7 4
1 7 1
3 4 2 4 3
1 2 2
1 6 5
1 4 0
1 2 4

*/