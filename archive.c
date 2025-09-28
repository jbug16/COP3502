#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 100

// Represents a player in the system
typedef struct {
    char *playerName;
    int totalScore;
} Player;

// Represents one puzzle instance (e.g., Wordle #751, Crossword #202)
typedef struct {
    char *puzzleType;
    int puzzleNo;
    Player** players;
    int *scores;
    int playerCount;
} Puzzle;

// Represents the archive of all puzzle instances
typedef struct {
    Puzzle* puzzles;
    int puzzleCount;
} Archive;

// Function definitions

// Reading
char** readPuzzleTypes(int *countOut);
Player** readPlayerPool(int *playerCount);
Puzzle** readPuzzles(int puzzleCount, Player** playerPool, int playerCount);

// Creating
Player *createPlayer(char *name);
Archive *createArchive(int puzzleCount);
Puzzle *createPuzzle(char* type, int puzzleNo, int playerCount);

// Finding
char* getPuzzleTypePtr(char **puzzleTypes, int count, char* type);
Player* getPlayerPtrByName(Player **playerPool, int playerCount, char *playerName);

// Printing
void printBestScorer(Puzzle *puzzle);
void printMaxTotalScorer(Archive *archive);

// Freeing
void freePuzzle(Puzzle *puzzle);
void freeArchive(Archive *archive);
void freePlayerPool(Player **pool, int count);



int main() {

    // 1. Number of types
    int puzzleTypeCount = 0;
    scanf("%d", &puzzleTypeCount);
    char** puzzleTypes = readPuzzleTypes(&puzzleTypeCount);

    // 2. Number of players
    int playerCount = 0;
    scanf("%d", &playerCount);
    Player** playerPool = readPlayerPool(&playerCount);

    // 3. Number of puzzles
    int puzzleCount = 0;
    scanf("%d", &puzzleCount);
    Puzzle** puzzles = readPuzzles(puzzleCount, playerPool, playerCount);

    // Create the archive
    Archive *archive = createArchive(puzzleCount);
    for (int i = 0; i < puzzleCount; i++) {
        archive->puzzles[i] = *puzzles[i];
    }

    // 4. Number of queries
    int numOfQueries = 0;
    scanf("%d", &numOfQueries);
    int* queries = malloc(numOfQueries * sizeof(int));

    // Get input first
    for (int i = 0; i < numOfQueries; i++) {
        scanf("%d", &queries[i]);
    }

    // Then print output
    for (int i = 0; i < numOfQueries; i++) {
        switch (queries[i]) {
            default: break;
            case 1:
                printMaxTotalScorer(archive);
                break;

            case 2:
                printf("Top scorer per puzzle:\n");
                for (int j = 0; j < puzzleCount; j++) {
                    printBestScorer(puzzles[j]);
                }
                break;
        }
    }

    // Free memory
    for (int i = 0; i < puzzleCount; i++) freePuzzle(puzzles[i]);
    free(puzzles);

    for (int i = 0; i < puzzleTypeCount; i++) free(puzzleTypes[i]);
    free(puzzleTypes);
    freeArchive(archive);

    freePlayerPool(playerPool, playerCount);

    free(queries);

    return 0;
}

// Read
char** readPuzzleTypes(int *countOut) {
    // countOut must be a valid memory address where the num of puzzle types can be stored

    int count = *countOut;
    char** str = malloc(count * sizeof(char*));

    // returns a dynamically allocated array of str loaded with pizzle types from input
    for (int i = 0; i < count; i++) {
        str[i] = malloc(MAX_STR_LEN * sizeof(char));
        scanf("%s", str[i]);
    }

    return str;
    // countOut is updated to reflect the num of puzzle types
}

Player** readPlayerPool(int *playerCount) {
    // playerCount must be a valid memory address where the num of players can be stored
    int count = *playerCount;

    // returns a dynamically allocated array of pointers to player structures
    // each pointer points to a dyn. alloc. player
    // playerCount is updated with the number of players read
    // must take the help from the createPlayer function

    Player** players = malloc(sizeof(Player*) * count);
    for (int i = 0; i < count; i++) {
        char name[MAX_STR_LEN];
        scanf("%s", name);
        players[i] = createPlayer(name);
    }

    *playerCount = count;
    return players;
}

Puzzle** readPuzzles(int puzzleCount, Player** playerPool, int playerCount) {
    // read in all puzzles and add player to said puzzle

    Puzzle** arr = malloc(sizeof(Puzzle*) * puzzleCount);

    for (int i = 0; i < puzzleCount; i++) { arr[i] = NULL; } // init

    for (int i = 0; i < puzzleCount; i++) {
        char type[MAX_STR_LEN];
        int puzzleNo;
        int count;

        // Get input for type, id and player count
        scanf("%s %d %d", type, &puzzleNo, &count);
        Puzzle* puzzle = createPuzzle(type, puzzleNo, count);

        // Read in each player and their score
        for (int j = 0; j < count; j++) {
            char name[MAX_STR_LEN];
            int score;

            scanf("%s %d", name, &score);

            // Find the player and add themselves to the puzzle's player list and score
            Player* player = getPlayerPtrByName(playerPool, playerCount, name);
            puzzle->scores[j] = score;
            puzzle->players[j] = player;
        }

        arr[i] = puzzle;
    }

    // return an array of puzzle pointers
    return arr;
}

// Create
Player *createPlayer(char *name) {
    Player *player = malloc(sizeof(Player));

    const size_t len = strlen(name) + 1; // len + 1 of name str
    player->playerName = malloc(len * sizeof(char));

    memcpy(player->playerName, name, len * sizeof(char));
    player->totalScore = 0;
    return player;
}

Archive* createArchive(int puzzleCount) {
    // puzzleCount represents the num of puzzles in the archive

    Archive *archive = malloc(sizeof(Archive));
    archive->puzzleCount = puzzleCount;
    archive->puzzles = malloc(sizeof(Puzzle) * puzzleCount);

    // Returns a pointer to a new archive structure containing an array of puzzle structures
    return archive;
}

Puzzle *createPuzzle(char* type, int puzzleNo, int playerCount) {
    Puzzle* puzzle = malloc(sizeof(Puzzle));

    const size_t len = strlen(type) + 1; // len of the name of the type (Wordle, etc)

    // allocate the memory for each variable in the struct
    puzzle->puzzleType = malloc(len * sizeof(char));
    puzzle->players = malloc(sizeof(Player*) * playerCount);
    puzzle->scores = malloc(sizeof(int) * playerCount);

    // set these variables given the input
    memcpy(puzzle->puzzleType, type, len * sizeof(char));
    puzzle->puzzleNo = puzzleNo;
    puzzle->playerCount = playerCount;

    // return a pointer to the created puzzle
    return puzzle;
}

// Find
char* getPuzzleTypePtr(char **puzzleTypes, int count, char* type) {
    // puzzleTypes must be a valid pointer to an array of strings
    // count should represent the num of puzzle types available
    // type must be a valid non-null string representing the type to find
    if (puzzleTypes == NULL) return NULL;
    if (type == NULL) return NULL;

    for (int i = 0; i < count; i++) {
        // returns a pointer to the matching puzzle type string, or NULL if not found
        if (strcmp(puzzleTypes[i], type) == 0) return puzzleTypes[i];
    }

    return NULL;
}

Player* getPlayerPtrByName(Player **playerPool, int playerCount, char *playerName) {
    // find the player given its name
    for (int i = 0; i < playerCount; i++) {
        if (strcmp(playerPool[i]->playerName, playerName) == 0) return playerPool[i];
    }

    // return the pointer to said player or NULL if they DNE
    return NULL;
}

// Print
void printBestScorer(Puzzle *puzzle) {
    // puzzle must be a valid pointer to a Puzzle struct with at least 1 player
    if (puzzle == NULL) return;
    if (puzzle->playerCount <= 0) {
        printf("%s#%d No player yet for this puzzle \n",
            puzzle->puzzleType,
            puzzle->puzzleNo);
        return;
    }

    // displays the name and score of the top player for given puzzle
    int bestScore = 0;
    int bestPlayerNo = 0;

    for (int i = 0; i < puzzle->playerCount; i++) {
        // on the case of a tie, the player who appears first is considered the best
        if (puzzle->scores[i] > bestScore) {
            bestScore = puzzle->scores[i];
            bestPlayerNo = i;
        }
    }

    printf("%s#%d %s %d\n",
                puzzle->puzzleType,
                puzzle->puzzleNo,
                puzzle->players[bestPlayerNo]->playerName,
                bestScore);
}

void printMaxTotalScorer(Archive* archive) {
    // archive must be a valid pointer to an Archive struct with puzzles

    if (archive == NULL || archive->puzzleCount <= 0) return;

    // displays the player with the highest total score
    // in case of a tie, the player who appears first is considered the top

    Player* topPlayer = NULL;
    int topScore = -1;

    // reset the total score for each player
    for (int i = 0; i < archive->puzzleCount; i++) {
        for (int j = 0; j < archive->puzzles[i].playerCount; j++) {
            archive->puzzles[i].players[j]->totalScore = 0;
        }
    }

    // add up score for each player per puzzle
    for (int i = 0; i < archive->puzzleCount; i++) {
        for (int j = 0; j < archive->puzzles[i].playerCount; j++) {
            // Get the current total score that each player has stored in their struct
            // Add the current score the player has for that puzzle
            archive->puzzles[i].players[j]->totalScore += archive->puzzles[i].scores[j];
        }
    }

    // find the max
    for (int i = 0; i < archive->puzzleCount; i++) {
        for (int j = 0; j < archive->puzzles[i].playerCount; j++) {
            if (archive->puzzles[i].players[j]->totalScore > topScore) {
                topPlayer = archive->puzzles[i].players[j];
                topScore = topPlayer->totalScore;
            }
        }
    }

    if (topPlayer != NULL) printf("Top player: %s with total score %d\n", topPlayer->playerName, topPlayer->totalScore);
}

// Free
void freePlayerPool(Player **pool, int count) {
    if (!pool) return; // safety check

    // free mem
    for (int i = 0; i < count; i++) {
        free(pool[i]->playerName);
        free(pool[i]);
    }
    free(pool);
}

void freeArchive(Archive *archive) {
    free(archive->puzzles);
    free(archive);
}

void freePuzzle(Puzzle *puzzle) {
    if (!puzzle) return;
    free(puzzle->players);
    free(puzzle->scores);
    free(puzzle->puzzleType);
    free(puzzle);
}