//
// Created by Jenna Curls on 12/2/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Entry{
    char word[6];
    int score;
} Entry;

typedef struct HeapStruct {
    Entry* heaparray;
    int capacity;
    int size;
} HeapStruct;

#define WORD_LEN 5

const int freq[26] = {
    /* a b c d e f g h i j k l m */
    8, 2, 3, 4, 13,3, 2, 6, 7, 0, 1, 4, 2,
    /* n o p q r s t u v w x y z */
    7, 7, 2, 0, 6, 6, 9, 3, 1, 2, 0, 2, 0
};

// Function definitions

    // Other helpers
    int calcScore(char* word);

    // Heaps
    HeapStruct* createHeap(int size);
    Entry* createEntry(char* word, int score);
    void heapify(HeapStruct* heap);
    void percolateDown(HeapStruct* heap, int index);
    void percolateUp(HeapStruct* heap, int index);
    int insert(HeapStruct* heap, char* word, int score);
    void swap(HeapStruct* heap, int index1, int index2);
    int maximum(HeapStruct* heap, int indexa, int indexb);
    void sort(HeapStruct* heap);
    void freeHeap(HeapStruct* heap);

    // Sorting and such
    int checkFeedback(char* word, char* guess, char* feedback);
    void filterDictionary(HeapStruct* heap, char** dictionary, int dictionarySize, char** guesses, int guessSize, char** feedbacks);

int main() {
    // Get input for dictionary
    int dictionarySize;
    scanf("%d", &dictionarySize);           // how many words in the dict

    char word[6];                           // temp storage for each word
    char** dictionary = malloc(dictionarySize * sizeof(char*)); // create whole dictionary
    for (int i = 0; i < dictionarySize; i++) {
        scanf("%s", word);                  // get input
        dictionary[i] = malloc(sizeof(char) * (WORD_LEN+1));
        strcpy(dictionary[i], word);        // copy word over to dictionary
    }

    // Get input for guesses
    int guessSize;
    scanf("%d", &guessSize);                // how many words the user is guessing

    char** guesses = malloc(guessSize * sizeof(char*));
    char** feedbacks = malloc(guessSize * sizeof(char*));
    for (int i = 0; i < guessSize; i++) {
        // Allocate
        guesses[i] = malloc(sizeof(char) * (WORD_LEN+1));
        feedbacks[i] = malloc(sizeof(char) * (WORD_LEN+1));

        scanf("%s", guesses[i]);            // get word input
        scanf("%s", feedbacks[i]);          // get feedback input
    }

    // Create heap
    HeapStruct* heap = createHeap(dictionarySize);

    // Filter words (add to said heap)
    filterDictionary(heap, dictionary, dictionarySize, guesses, guessSize, feedbacks);

    // Heap sort
    sort(heap);

    // Clean up
        // Dictionary
        for (int i = 0; i < dictionarySize; i++) {
            free(dictionary[i]);
        }
        free(dictionary);

        // Guesses and feedbacks
        for (int i = 0; i < guessSize; i++) {
            free(guesses[i]);
            free(feedbacks[i]);
        }
        free(guesses);
        free(feedbacks);

        // Heap
        freeHeap(heap);

    return 0;
}

// Calculate the score of the word
int calcScore(char* word) {
    int score = 0;

    for (int i = 0; i < WORD_LEN; i++) {
        int points = freq[word[i]-'a'];
        score += points;
    }

    return score;
}

// Create an entry for the heap storing the word and it's score
Entry* createEntry(char* word, int score) {
    // Allocate mem for each entry
    Entry* entry = malloc(sizeof(Entry));

    strcpy(entry->word, word); // copy the input to the mem
    entry->score = score;      // set word's score

    return entry;
}

// Heap stuff from heap.c

HeapStruct* createHeap(int size) {

    // Allocate space for the heap and set the size for an empty heap.
    HeapStruct* h = (malloc(sizeof(HeapStruct)));
    h->capacity = size;
    h->heaparray = (Entry*)malloc(sizeof(Entry)*(size+1));
    h->size = 0;
    return h;
}

HeapStruct* initHeapfromArray(Entry* entries, int length) {

    HeapStruct* h = (malloc(sizeof(HeapStruct)));

    // We allocate one extra slot, since slot 0 stays unused.
    h->heaparray = (Entry*)malloc(sizeof(Entry)*(length+1));

    // Just copy the values into our array.
    for (int i=1; i<=length; i++) {
        h->heaparray[i] = entries[i-1];
    }

    // This is the number of values we copied.
    h->size = length;

    // This takes our random values and rearranges them into a heap.
    heapify(h);
    return h;
}

void heapify(HeapStruct* heap) {
    // We form a heap by just running percolateDown on the first half of the
    // elements, in reverse order.
    for (int i=heap->size/2; i>0; i--)
        percolateDown(heap, i);
}

Entry removeMax(HeapStruct *h) {

    Entry retval;

    // We can only remove an element, if one exists in the heap!
    if (h->size > 0) {

        // This is where the max is stored.
        retval = h->heaparray[1];

        // Copy the last value into this top slot.
        h->heaparray[1] = h->heaparray[h->size];

        // Our heap will have one fewer items.
        h->size--;

        // Need to let this value move down to its rightful spot in the heap.
        percolateDown(h, 1);

        // Now we can return our value.
        return retval;
    }

    // No value to return, indicate failure with a -1.
    else {
        Entry* empty = createEntry("", 0); // basically just making an empty entry object so we can return something
        return *empty;
    }
}

void percolateDown(HeapStruct* heap, int index) {
    int max;

    // Only try to percolate down internal nodes.
    if ((2*index+1) <= heap->size) {

        // Find the max value of the two children of this node.
        max = maximum(heap, 2*index, 2*index+1);

        // Shorthands
        Entry a = heap->heaparray[index];
        Entry b = heap->heaparray[max];

        // If this value is less than the current value, then we need to move
        // our current value down the heap.
        if (a.score < b.score) {
            swap(heap, index, max);

            // This part is recursive and allows us to continue percolating
            // down the element in question.
            percolateDown(heap, max);
        }
        // Tie: determine alphabetically
        else if (a.score == b.score && strcmp(a.word, b.word) > 0)
        {
            swap(heap, index, max);
            percolateDown(heap, max);
        }
    }

    // Case where our current element has exactly one child, a left child.
    else if (heap->size == 2*index) {

        // Here we only compare the current item to its only child.
        // Clearly, no recursive call is needed since the child of this node
        // is a leaf.
        if (heap->heaparray[index].score < heap->heaparray[2*index].score)
            swap(heap, index, 2*index);
    }
}

void percolateUp(HeapStruct* heap, int index) {

    // Can only percolate up if the node isn't the root.
    if (index > 1) {

        // Shorthands
        Entry a = heap->heaparray[index/2];
        Entry b = heap->heaparray[index];

        // See if our current node is smaller in value than its parent.
        if (a.score < b.score) {

            // Move our node up one level.
            swap(heap, index, index/2);

            // See if it needs to be done again.
            percolateUp(heap, index/2);
        }
        // Tie: determine alphabetically
        else if (a.score == b.score && strcmp(a.word, b.word) > 0)
        {
            swap(heap, index, index/2);
            percolateUp(heap, index/2);
        }
    }
}

int insert(HeapStruct* heap, char* word, int score) {

    // Our array is full, we need to allocate some new space!
    if (heap->size == heap->capacity) {

        // Allocate new space for an array.
        heap->heaparray = (Entry*)realloc(heap->heaparray, sizeof(Entry)*(2*heap->capacity+1));

        // Realloc failed so we quit.
        if (heap->heaparray == NULL) return 0;

        // Double the capacity.
        heap->capacity *= 2;
    }

    // Adjust all the necessary components of h, and then move the inserted
    // item into its appropriate location.
    heap->size++;
    strcpy(heap->heaparray[heap->size].word, word);
    heap->heaparray[heap->size].score = score;
    percolateUp(heap, heap->size);

    return 1;
}

void swap(HeapStruct* heap, int index1, int index2) {
    Entry temp = heap->heaparray[index1];
    heap->heaparray[index1] = heap->heaparray[index2];
    heap->heaparray[index2] = temp;
}

int maximum(HeapStruct* heap, int indexa, int indexb) {

    int a = heap->heaparray[indexa].score;
    int b = heap->heaparray[indexb].score;

    // Return the value associated with a.
    if (a > b)
        return indexa;

    // Return the value associated with b.
    else if (a < b)
        return indexb;

    // scores are equal (sort alphabetically instead)
    else {
        if (strcmp(heap->heaparray[indexa].word, heap->heaparray[indexb].word) < 0)
            return indexa;
        else
            return indexb;
    }
}

void sort(HeapStruct* heap) {

    // Create a heap from the array of values.
    int length = heap->size;

    // This heap is empty
    if (length == 0) {
        printf("No candidates found.\n");
        return;
    }

    // Remove these values from the heap one by one and store them back in the
    // original array.
    for (int i=0; i<length; i++) {
        Entry entry = removeMax(heap);
        printf("%d %s\n", entry.score, entry.word);
    }

}

void freeHeap(HeapStruct* heap) {
    free(heap->heaparray);
    free(heap);
}


// Check if the feedback is equal to the expected feedback
int checkFeedback(char* word, char* guess, char* feedback) {
    // word is the current word in the dictionary
    // guess is the word the user is playing
    // feedback is the green/yellow/gray stuff
    // we are going to do 2 loops through the word (first checking green then yellow spots) b/c
    // yellow is for the leftover letters. so if there is still an unused letter then it's yellow
    // hopefully that makes sense :p

    // Create feedback for the current word based on the guess
    char otherFeedback[WORD_LEN+1];
    int used[WORD_LEN] = {0};

    // 1st loop through the word (first check same letters/pos)
    for (int i = 0; i < WORD_LEN; i++) {
        // Init all feedback to gray
        otherFeedback[i] = 'B';

        // Check if this is the same letter and position
        if (guess[i] == word[i]) {
            otherFeedback[i] = 'G'; // green
            used[i] = 1;            // this answer position is used
        }
    }

    // 2nd loop through the word (now check same letter, dif pos)
    for (int i = 0; i < WORD_LEN; i++) {
        // Skip if green
        if (otherFeedback[i] == 'G') continue;

        // Check if the unused letters may be somewhere else in the word (not right position)
        for (int j = 0; j < WORD_LEN; j++) {
            if (used[j] == 0 && guess[i] == word[j]) {
                otherFeedback[i] = 'Y'; // yellow
                used[j] = 1;            // this answer position is used
                break;
            }
        }
    }
    otherFeedback[WORD_LEN] = '\0'; // end string

    // Compare the new feedback for this dictionary word to the one the user input for word
    if (strcmp(otherFeedback, feedback) == 0) return 1; // perfect match
    else return 0;                                      // not the same letters/positions
}

// Filter the dictionary based on the feedback and insert words into the heap
void filterDictionary(HeapStruct* heap, char** dictionary, int dictionarySize, char** guesses, int guessSize, char** feedbacks) {
    // this function will loop through all words in the dictionary and all guesses and compare them
    // then will add a "good" word into the heap (good word as in it matches all the guesses)
    // this is o(n) because n is the # of words in the dictionary and guessSize is a constant so o(n*2) = o(n) (right?)

    // Loop through dictionary
    for (int i = 0; i < dictionarySize; i++) {
        char* word = dictionary[i]; // this is the "answer" aka just the word we're checking
        int passed = -1;            // init passed

        // For every guess
        for (int j = 0; j < guessSize; j++) {
            // Check if this word is NOT a good next guess
            if (!checkFeedback(word, guesses[j], feedbacks[j])) {
                passed = 0; // fail, bad word
                break;
            }
            else passed = 1; // passed, good word
        }

        // If this word passes our check (otherwise, just skip it)
        if (passed) {
            // Score it
            int score = calcScore(word);

            // Add it to the heap
            insert(heap, word, score);
        }
    }
}


// sorry but i started this around 12:30pm day of and now it's 11:45pm my eyes are burning so bad
// hopefully this all makes sense and i didn't miss anything!!!!!!