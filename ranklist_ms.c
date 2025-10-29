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
void mergeSort(Player** list, int n, int key);
void mergeSortRec(Player** list, int low, int high, int key);
int isSorted(int values[], int length);

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

// Merge Sorts the array list of size n according to the game
// indicated by the integer key.
void mergeSort(Player** list, int n, int key) {
    //printf("merge %d, %d, %d\n", start, middle, end);

    int *temp, i, length, count1, count2, mc;

    // Allocate the proper amount of space for our auxiliary array.
    length = end - start + 1;
    temp = (int*)calloc(length, sizeof(int));

    // These will be our indexes into our two sorted lists.
    count1 = start;
    count2 = middle;

    // Keeps track of our index into our auxiliary array.
    mc = 0;

    // Here we copy values into our auxiliary array, so long as there are
    // numbers from both lists to copy.
    while ((count1 < middle) || (count2 <= end)) {

        // Next value to copy comes from list one - make sure list
        // one isn't exhausted yet. Also make sure we don't access index
        // count2 if we aren't supposed to.
        if (count2 > end || (count1 < middle && values[count1] < values[count2])) {
            temp[mc] = values[count1];
            count1++;
            mc++;
        }

        // We copy the next value from list two.
        else {
            temp[mc] = values[count2];
            count2++;
            mc++;
        }
    }

    // Copy back all of our values into the original array.
    for (i=start; i<=end; i++)
        values[i] = temp[i - start];

    // Don't need this space any more!
    free(temp);
}

// Performs a Merge Sort on list[low..high] according to the game
// indicated by the integer key.
void mergeSortRec(Player** list, int low, int high, int key) {
    // Check if our sorting range is more than one element.
    if (low < high) {

        int mid = (low + high) / 2;

        // Sort the first half of the values.
        mergeSortRec(list, low, mid, key);

        // Sort the last half of the values.
        mergeSortRec(list, mid+1, high, key);

        // Put it all together.
        mergeSort(list, low, mid+1, high, key);
    }
}

int isSorted(int values[], int length) {
    // Return false if any adjacent pair is out of order.
    for (int i = 0; i < length-1; i++) {
        if (values[i] > values[i+1]) return 0;
    }

    return 1;
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