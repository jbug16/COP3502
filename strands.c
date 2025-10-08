//
// Created by Jenna Curls on 10/7/2025.
//

#include <stdio.h>

typedef struct dllnode {
    char ch;
    struct dllnode* prev;
    struct dllnode* next;
} dllnode;

int main() {
    // 1. Print out the word from that grid position
    // 2. Print out the reverse of that word from the grid position
    // 3. If the current position is the end of a word AND an adjacent position is a starting letter, merge those words together

    const int NUMDIR = 8;
    const int NULLPTR = -1;
    const int DR[NUMDIR] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int DC[NUMDIR] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    int numRows;
    int numCols;
    dllnode*** strandsGrid;

    // Get input for rows and cols (ex: "8 6")
    // Get input for letters (ex: 8 lines of input, 6 characters each line, no space)
    // Get input for direction of each letter's next letter (if -1, this is the LAST letter in a word)
    // Get input for direction for previous letter (if -1, this is the FIRST letter in a word)
    // Get input for how many queries to expect
        // 1. Print the word that the node in a particular grid square is part of.
        // 2. Reverse the word that the node in a particular grid square is part of.
        // 3. Given the position of the last letter in one word, and an adjacent position that is the starting letter
        // in a different word, concatenate the word in the first position to the word in the second listed position.
    // Get input (for each query) of what function to call & arguments

    // (Output for query 1 only) Print the word that grid position is apart of

    return 0;
}