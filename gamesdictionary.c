//
// Created by Jenna Curls on 11/8/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 19
#define NUMGAMES 6

typedef struct NYT_String {
    char* str;
    int allowed[NUMGAMES];
} NYT_String;

typedef struct BST_Node {
    NYT_String* ptr;
    struct BST_Node* left;
    struct BST_Node* right;
} BST_Node;

// Function definitions
NYT_String* createString(const char* word, int game);
BST_Node* createNode(const char* word, int game);
BST_Node* insert(BST_Node* root, BST_Node* node);
BST_Node* find(BST_Node* curr, char* word);

BST_Node* addWord(BST_Node* root, int game, char* word);
BST_Node* deleteWord(BST_Node* root, int game, char* word);
void whichGame(char* word);
char** allStringsInGame(BST_Node* root, int gameNo, int* arrSize);
void sameLengthWords(int game, int length);
void nextWord(BST_Node* root);

void printTree(BST_Node* root);
void printGames(BST_Node* root);

/*
1. Add a string to a game.
2. Remove a string from a game.
3. Given a string, determine which game(s), if any, it's allowed in.
4. Return an alphabetically sorted list of all strings allowed for a particular game.
5. Return the number of strings of a particular length allowed in a particular game.
6. Given a string, return the next string alphabetically, that appears in any game.
*/

int main() {
    // Create tree
    BST_Node* root = NULL;

    // Get input
    int numOfOps;
    scanf("%d", &numOfOps);

    for (int i = 0; i < numOfOps; i++) {
        // First line is the type of query/operation
        int op;
        scanf("%d", &op);

        // Determine the data to print depending on the requested operation
        int game, length;
        char str[MAXSIZE+1];
        switch (op) {
            // ex: 1 0 hello (add string "hello" to game 0)
            case 1:
                scanf("%d %s", &game, str);
                root = addWord(root, game, str);
                break;

            // ex: 2 0 hello (delete string "hello" from game 0)
            case 2:
                scanf("%d %s", &game, str);
                deleteWord(root, game, str);
                break;

            // ex: 3 bye (checks if "bye" is in any of the games, return -1 if no)
            case 3:
                scanf("%s", str);
                whichGame(str);
                break;

            // ex: 4 0 (returns all strings in game 0)
            case 4:
                scanf("%d", &game);
                //allStringsInGame
                break;

            // ex: 5 0 10 (return num of strings with the length of 10 in game 0)
            case 5:
                scanf("%d %d", &game, &length);
                sameLengthWords(game, length);
                break;

            // ex: 6 hello (return the next string alphabetically)
            case 6:
                scanf("%s", str);
                //nextWord(str);
                break;

            default:
                break;
        }
    }

    BST_Node* found = find(root, "hello");
    if (!found) printf("NULL\n");
    if (!root->left) printf("left NULL\n");
    if (!root->right) printf("right NULL\n");
    printTree(root);
    printf("\n");
    printGames(root);

    // Clean up

    return 0;
}

// Helper functions
NYT_String* createString(const char* word, const int game) {
    // Init the nyt data
    NYT_String* nyt = malloc(sizeof(NYT_String));
    nyt->str = malloc(sizeof(char) * (strlen(word) + 1));  // the actual word
    strcpy(nyt->str, word);                                     // copy because that's how c work ig
    for (int i = 0; i < NUMGAMES; i++)                          // set all games to false (0)
        nyt->allowed[i] = 0;
    nyt->allowed[game] = 1;                                     // sets this game's index to true (1)

    return nyt; // return pointer to the nyt data
}

BST_Node* createNode(const char* word, const int game) {
    // Init BST node
    BST_Node* node = malloc(sizeof(BST_Node));
    node->left = NULL;
    node->right = NULL;
    node->ptr = createString(word, game); // this is the pointer to the nyt data

    return node;
}

BST_Node* insert(BST_Node* root, BST_Node* node) {
    if (root == NULL) return node;

    // Recursively insert on left.
    if (strcmp(node->ptr->str, root->ptr->str) < 0) {
        // Recursively insert and reassign left.
        root->left = insert(root->left, node);
    }
    // Must go right if we get here.
    else if (strcmp(node->ptr->str, root->ptr->str) > 0) {
        // Recursively insert and reassign right.
        root->right = insert(root->right, node);
    }

    // This is still the root of the tree.
    return root;
}

BST_Node* find(BST_Node* curr, char* word) {

    // Check if there are nodes in the tree.
    if (curr != NULL) {

        // Found the value at the root.
        if (strcmp(word, curr->ptr->str) == 0)
            return curr;

        // Search to the left.
        if (strcmp(word, curr->ptr->str) < 0)
            return find(curr->left, word);

        // Or...search to the right.
        if (strcmp(word, curr->ptr->str) > 0)
            return find(curr->right, word);
    }

    // Null case.
    return NULL;
}

// Operation functions
BST_Node* addWord(BST_Node* root, int game, char* word) {
    // Debug
    printf("Adding \"%s\" to game %d...\n", word, game);

    // Does the node already exist?
    BST_Node* node = find(root, word);

    // Check if this word is in a game
    if (node != NULL) {
        // Just add it to the new game
        node->ptr->allowed[game] = 1;
    }
    // If not
    else {
        // Create this word
        node = createNode(word, game);

        // Inset this node into the tree
        root = insert(root, node);
    }

    return root;
}

BST_Node* deleteWord(BST_Node* root, int game, char* word) {
    // Debug
    printf("Deleting \"%s\" from game %d...\n", word, game);

    
    return NULL;
}

void whichGame(char* word) {
    printf("\"%s\" is in game ?...\n", word);
}

char** allStringsInGame(BST_Node* root, int gameNo, int* arrSize) {

    return NULL;
}

void sameLengthWords(int game, int length) {
    printf("same length...\n");
}

void nextWord(BST_Node* root) {

}

// Debug
void printTree(BST_Node* root) {
    if (root == NULL) return;
    printTree(root->left);
    printf("%s ", root->ptr->str);
    printTree(root->right);
}

void printGames(BST_Node* root) {
    if (root == NULL) return;
    for (int i = 0; i < NUMGAMES; i++) {
        printf("%d ", root->ptr->allowed[i]);
    }
}