//
// Created by Jenna Curls on 9/16/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LETTER_MAX 7
#define ALPHA_SIZE 26

// Global vars
char* letters;
int word_num;
char** dictionary;
int globalCNT = 0;

// Function definitions
char* sortLetters(char* letters);
void print(char* array, int n);
void printPerms(char* original, char* perm, char* used, int k, int n, int minLen, int maxLen);
char* filterPerm(char* perm, char reqLetter);
int isInDictionary(char* word);
int binsearch(char** words, int low, int high, char* target);
int sortWords(const void* a, const void* b);

int main() {
    // Get input for letters
    letters = malloc(sizeof(char) * (LETTER_MAX + 1));
    scanf("%7s", letters);

    // Get input for number of words
    scanf("%d", &word_num);

    // Get input for words
    dictionary = malloc(sizeof(char*) * word_num);
    for (int i = 0; i < word_num; i++) {
        dictionary[i] = malloc(sizeof(char) * (LETTER_MAX + 1));
        scanf("%s", dictionary[i]);
    }

    // Sort the dictionary of words
    qsort(dictionary, word_num, sizeof(char*), sortWords);

    // Allocate memory for helper variables
    char* used = calloc((LETTER_MAX + 1), sizeof(char)); // string of used letters for perm
    char* sorted = sortLetters(letters); // string of sorted letters
    char* perm = calloc(LETTER_MAX + 1, sizeof(char)); // string of letters mixed up (perm)

    // Run the perm and print the output
    printPerms(sorted, perm, used, 0, 7, 4, 7);

    // Free all the memory
    free(letters);
    free(used);
    for (int i = 0; i < word_num; i++) {
        free(dictionary[i]);
    }
    free(dictionary);
    free(sorted);
    free(perm);

    return 0;
}

// Sort the letters into alphabetic order using the counting sort algorithm.
// Returns a new string of the sorted letter.
char* sortLetters(char* letters) {
    // Using a counting sort algorithm
    char* sorted = malloc(sizeof(char) * (LETTER_MAX + 1));
    int* count = calloc(ALPHA_SIZE, sizeof(int));

    // Count how many letters
    for (int i = 0; i < LETTER_MAX; i++) {
        count[letters[i] - 'a']++;
    }

    int index = 0;
    // Go through each letter of the alphabet
    for (int i = 0; i < ALPHA_SIZE; i++) {
        // Loop through the letter as many times as the letter shows up in the string
        for (int j = 0; j < count[i]; j++) {
            // Add the letter j times to a new array
            sorted[index++] = 'a' + i;
        }
    }

    // Clean up
    sorted[index] = '\0';
    free(count);

    return sorted;
}

// A helper function from bruteforce.c in class.
// Prints the contents of an array.
void print(char* array, int n) {
    for (int i = 0; i < n; i++)
        printf("%c", array[i]);
    printf("\n");
}

// Based off the printPerms function from bruteforce.c in class.
// Prints all permutations of the original string that is between 4-7 letters long and includes the required letter.
void printPerms(char* original, char* perm, char* used, int k, int n, int minLen, int maxLen) {
    // Base case.
    if (k >= minLen) {
        perm[k] = '\0';
        char* filteredPerm = filterPerm(perm, letters[3]);
        if (filteredPerm) print(filteredPerm, k);
    }

    // Stop recursion if the word length is already 7.
    if (k == maxLen) return;

    // Only fill slot k with items that have yet to be used. If i hasn't been used,
    // put it in slot k and recursively print all permutations with these k+1 starting items.
    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            used[i] = 1;
            perm[k] = original[i];
            printPerms(original, perm, used, k+1, n, minLen, maxLen);
            used[i] = 0;
        }
    }
}

// Returns a perm that has the required letter in the word.
char* filterPerm(char* perm, char reqLetter) {
    int len = strlen(perm);
    for (int i = 0; i < len; i++) {
        if (perm[i] == reqLetter) {
            if (isInDictionary(perm)) return perm;
        }
    }
    return NULL;
}

// Uses binary search to verify the perm is in the dictionary the user defined.
int isInDictionary(char* word) {
    return binsearch(dictionary, 0, word_num-1, word) != -1;
}

// Binary search algorithm from binsearch.c.
int binsearch(char** words, int low, int high, char* target) {

    // Empty search space.
    if (low > high) return -1;

    int mid = (low+high)/2;

    // Save this since we need it twice.
    int cmp = strcmp(target, words[mid]);
    globalCNT++;

    // target must be in left side of array.
    if (cmp < 0)
        return binsearch(words, low, mid-1, target);

    // right side...
    else if (cmp > 0)
        return binsearch(words, mid+1, high, target);

    // We found it!
    else
        return mid;
}

// A helper function for the qsort function from GeeksForGeeks.
int sortWords(const void* a, const void* b) {
    return strcmp(*(const char **)a, *(const char **)b);
}