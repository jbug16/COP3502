//
// Created by Jenna Curls on 10/17/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 12
#define NUMGAMES 7
#define TOTAL 6
#define BASECASESIZE 30

const char GAMES[NUMGAMES][MAXSIZE+1] = {"Wordle", "Spelling Bee", "Crossword", "Connections", "Strands", "Letter Boxed", "Total"};

typedef struct Player {
    char* name;
    int scores[NUMGAMES];
} Player;

// Function definitions
Player* createNewPlayer();
void printScores(Player* player);
void printList(int playerCount, Player** list, int game);
int compare(Player* ptrP1, Player* ptrP2, int key);
int isSorted(Player** list, int low, int high, int game);
void swap(Player** a, Player** b);
void bubbleSort(Player** list, int n, int key);
void mergeSort(Player** list, int n, int key);
void mergeSortRec(Player** list, int low, int high, int key);
void merge(Player** list, int low, int mid, int high, int key);

int main() {
    // How many players are there
    int playerCount;
    scanf("%d", &playerCount);

    // Create list of players
    Player** players = malloc(sizeof(Player*) * playerCount);
    for (int i = 0; i < playerCount; i++) {
        // Create the individual player
        players[i] = createNewPlayer();

        // Get and set player name
        scanf("%s", players[i]->name);

        // Get score for each game
        int total = 0;
        for (int j = 0; j < TOTAL; j++) {
            scanf("%d", &players[i]->scores[j]);
            total += players[i]->scores[j]; // calc total
        }

        // Set total
        players[i]->scores[TOTAL] = total;
    }

    // What game are we trying to rank
    int game;
    scanf("%d", &game);

    // Sort the list (ranking them)
    mergeSort(players, playerCount, game);

    // Print
    printList(playerCount, players, game);

    // Clean up
    for (int i = 0; i < playerCount; i++) {
        free(players[i]->name);
        free(players[i]);
    }
    free(players);

    return 0;
}

// Create a player 'object' and set all scores to 0, init all player variables
Player* createNewPlayer() {
    Player* player = malloc(sizeof(Player)); // player object

    player->name = malloc((MAXSIZE+1) * sizeof(char)); // name

    // Init each score (and total) to 0
    for (int i = 0; i < NUMGAMES; i++) {
        player->scores[i] = 0;
    }

    return player;
}

// Debug print
void printScores(Player* player) {
    printf("%s\n", player->name);
    for (int i = 0; i < TOTAL; i++) {
        printf("\t%s = %d\n", GAMES[i], player->scores[i]);
    }
    printf("\tTotal = %d\n", player->scores[TOTAL]);
}

// Print the ranked list
void printList(int playerCount, Player** list, int game) {
    printf("%s Ranklist\n", GAMES[game]);
    for (int i = 0; i < playerCount; i++) {
        printf("%d. %-15s %d\n", i+1, list[i]->name, list[i]->scores[game]);
    }
}

// Returns a negative integer if the player pointed to by ptrP1
// "comes before" the player pointed to by ptrP2 on the ranklist
// sorted by the game indicated by the integer key, breaking ties
// by the player's name in alphabetical order. Returns 0 is the
// two players are identical and returns a positive integer if the
// player pointed to by ptrP2 comes before the player pointed to
// by ptrP1.
int compare(Player* ptrP1, Player* ptrP2, int key) {
    // If player 1 comes before player 2
    if (ptrP1->scores[key] > ptrP2->scores[key]) return -1;
    // If player 2 comes before player 1
    if (ptrP1->scores[key] < ptrP2->scores[key]) return 1;

    // If the scores are the same, check if the names are
    // Player 1's name comes first
    if (strcmp(ptrP1->name, ptrP2->name) < 0) return -1;
    // Player 2's name comes first
    if (strcmp(ptrP1->name, ptrP2->name) > 0) return 1;
    // Identical
    return 0;
}

// Returns whether the list is sorted or not
int isSorted(Player** list, int low, int high, int game) {
    // Return false if any adjacent pair is out of order.
    for (int i = low; i < high; i++) {
        if (compare(list[i], list[i+1], game) > 0) return 0;
    }

    return 1;
}

// Swaps the players pointed to by a and b.
void swap(Player** a, Player** b) {
    Player* temp = *a;
    *a = *b;
    *b = temp;
}

// Bubble sort algorithm from sort.c
void bubbleSort(Player** list, int n, int key) {
    // i is how far we go in the inner loop.
    for (int i=n-2; i>=0; i--) {
        // j battles j+1
        for (int j=0; j<=i; j++) {
            // Out of order, swap.
            if (compare(list[j], list[j+1], key) > 0) {
                swap(&list[j], &list[j+1]);
            }
        }
    }
}

// Merge Sorts the array list of size n according to the game
// indicated by the integer key.
void mergeSort(Player** list, int n, int key) {
    mergeSortRec(list, 0, n - 1, key);
}

// Performs a Merge Sort on list[low..high] according to the game
// indicated by the integer key.
void mergeSortRec(Player** list, int low, int high, int key) {
    // Check if our sorting range is more than one element.
    if (low < high) {

        // Hybrid base case - use bubble sort if small enough
        if (high - low + 1 <= BASECASESIZE) {
            bubbleSort(list + low, high - low + 1, key);
            return;
        }

        int mid = (low + high) / 2;

        // Sort the first half of the values.
        mergeSortRec(list, low, mid, key);

        // Sort the last half of the values.
        mergeSortRec(list, mid+1, high, key);

        // Put it all together.
        merge(list, low, mid+1, high, key);
    }
}

// Actually doing the merging. From notes.
void merge(Player** list, int low, int mid, int high, int key) {
    // Allocate the proper amount of space for our auxiliary array.
    int length = high - low + 1;
    Player** temp = calloc(length, sizeof(Player*));

    // These will be our indexes into our two sorted lists.
    int count1 = low;
    int count2 = mid;

    // Keeps track of our index into our auxiliary array.
    int mc = 0;

    // Here we copy values into our auxiliary array, so long as there are
    // numbers from both lists to copy.
    while ((count1 < mid) || (count2 <= high)) {

        // Next value to copy comes from list one - make sure list
        // one isn't exhausted yet. Also make sure we don't access index
        // count2 if we aren't supposed to.
        if (count2 > high || (count1 < mid && compare(list[count1], list[count2], key) <= 0)) {
            temp[mc] = list[count1];
            count1++;
            mc++;
        }

        // We copy the next value from list two.
        else {
            temp[mc] = list[count2];
            count2++;
            mc++;
        }
    }

    // Copy back all of our values into the original array.
    for (int i = low; i <= high; i++)
        list[i] = temp[i - low];

    // Don't need this space anymore!
    free(temp);
}