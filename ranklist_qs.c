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
void printList(int playerCount, Player** players, int game);
int compare(Player* ptrP1, Player* ptrP2, int key);
int isSorted(int values[], int low, int high);
void swap(int *a, int *b);
int partition(Player** players, int low, int high);
void quicksort(Player** players, int low, int high);

int main() {
    // Get input and create players/set scores
    int playerCount;
    scanf("%d", &playerCount);

    Player** players = malloc(sizeof(Player*) * playerCount);
    for (int i = 0; i < playerCount; i++) {
        // Create the player
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

    int game;
    scanf("%d", &game);

    // Sort
    quicksort(players, 0, TOTAL);

    // Test print
    printList(playerCount, players, game);

    // Clean up
    for (int i = 0; i < playerCount; i++) {
        free(players[i]);
    }
    free(players);

    return 0;
}

Player* createNewPlayer() {
    Player* player = malloc(sizeof(Player));
    player->name = malloc(MAXSIZE * sizeof(char));
    for (int i = 0; i < TOTAL; i++) {
        player->scores[i] = 0;
    }

    return player;
}

void printScores(Player* player) {
    printf("%s\n", player->name);
    for (int i = 0; i < TOTAL; i++) {
        printf("\t%s = %d\n", GAMES[i], player->scores[i]);
    }
    printf("\tTotal = %d\n", player->scores[TOTAL]);
}

// Print the ranked list
void printList(int playerCount, Player** players, int game) {
    for (int i = 0; i < playerCount; i++) {
        printf("%d. %-15s %d\n", i+1, players[i]->name, players[i]->scores[game]);
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

int isSorted(int values[], int low, int high) {
    // Return false if any adjacent pair is out of order.
    for (int i = low; i < high; i++) {
        if (values[i] > values[i+1]) return 0;
    }

    return 1;
}

// Swaps the values pointed to by a and b.
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Player** players, int low, int high) {
    // Pick a random partition element and swap it into index low.
    int i = low + rand() % (high-low+1);
    swap(&vals[low], &vals[i]);

    // Store the index of the partition element.
    int lowpos = low;
    low++;

    // Run the partition so long as the low and high counters don't cross.
    while (low <= high) {
        // Move the low pointer until we find a value too large for this side.
        while (low <= high && vals[low] <= vals[lowpos]) low++;

        // Move the high pointer until we find a value too small for this side.
        while (high >= low && vals[high] > vals[lowpos]) high--;

        // Now that we've identified two values on the wrong side, swap them.
        if (low < high) swap(&vals[low], &vals[high]);
    }

    // Swap the partition element into it's correct location.
    swap(&vals[lowpos], &vals[high]);

    return high; // Return the index of the partition element.
}

void quicksort(int* numbers, int low, int high) {
    // If already sorted, just exit
    if (isSorted(numbers, low, high)) return;

    // Split the array to sort each side
    int split = partition(numbers, low, high);
    quicksort(numbers, low, split - 1);
    quicksort(numbers, split + 1, high);
}

/*
7
james 200 100 50 300 500 700
marnie 500 50 50 50 700 300
abbie 100 100 100 100 100 100
zach 0 200 500 800 300 100
emery 400 300 200 100 100 0
mabel 50 40 30 80 70 90
dave 600 200 300 100 0 0
3
*/