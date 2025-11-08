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
int partition(Player** list, int low, int high, int game);
void quickSortRec(Player** list, int low, int high, int key);
void quickSort(Player** list, int n, int key);

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
    quickSort(players, playerCount, game);

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
    printf("%s\n", player->name); // name
    for (int i = 0; i < TOTAL; i++) {
        printf("\t%s = %d\n", GAMES[i], player->scores[i]); // each score for each game
    }
    printf("\tTotal = %d\n", player->scores[TOTAL]); // total score
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
        if (compare(list[i], list[i+1], game) > 0)
            return 0; // false
    }

    return 1; // true
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

// Splits the list into a bunch of sublists and swaps pointers to sort the list
int partition(Player** list, int low, int high, int game) {
    // Pick a random partition element and swap it into index low.
    int i = low + rand() % (high - low + 1);
    swap(&list[low], &list[i]);

    // Store the index of the partition element.
    int lowpos = low;
    low++;

    // Run the partition so long as the low and high counters don't cross.
    while (low <= high) {
        // Move the low pointer until we find a value too large for this side.
        while (low <= high && compare(list[low], list[lowpos], game) <= 0) low++;

        // Move the high pointer until we find a value too small for this side.
        while (high >= low && compare(list[high], list[lowpos], game) > 0) high--;

        // Now that we've identified two values on the wrong side, swap them.
        if (low < high) swap(&list[low], &list[high]);
    }

    // Swap the partition element into it's correct location.
    swap(&list[lowpos], &list[high]);

    return high; // Return the index of the partition element.
}

// Performs a Quick Sort on list[low..high] according to the game
// indicated by the integer key.
void quickSortRec(Player** list, int low, int high, int key) {
    // If already sorted, just exit
    if (isSorted(list, low, high, key)) return;

    // Hybrid base case - use bubble sort if small enough
    if (high - low + 1 <= BASECASESIZE) {
        bubbleSort(list + low, high - low + 1, key);
        return;
    }

    // Split the array to sort each side
    int split = partition(list, low, high, key);
    quickSortRec(list, low, split - 1, key);
    quickSortRec(list, split + 1, high, key);
}

// Quick Sorts the array list of size n according to the game
// indicated by the integer key.
void quickSort(Player** list, int n, int key) {
    quickSortRec(list, 0, n - 1, key);
}