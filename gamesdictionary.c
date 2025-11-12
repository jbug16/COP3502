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
int isLeaf(BST_Node* node);
int hasOnlyLeftChild(BST_Node* node);
int hasOnlyRightChild(BST_Node* node);
BST_Node* minVal(BST_Node* root);
BST_Node* removeFromGame(BST_Node* root, int game, char* word);
BST_Node* deleteNode(BST_Node* root, char* word);
void freeNode(BST_Node* root);

BST_Node* addWord(BST_Node* root, int game, char* word);
BST_Node* deleteWord(BST_Node* root, int game, char* word);
void whichGame(BST_Node* root, char* word);
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
                root = deleteWord(root, game, str);
                break;

            // ex: 3 bye (checks if "bye" is in any of the games, return -1 if no)
            case 3:
                scanf("%s", str);
                whichGame(root, str);
                break;

            // ex: 4 0 (returns all strings in game 0)
            case 4:
                scanf("%d", &game);
                int arrSize;
                char** allStrings = allStringsInGame(root, game, &arrSize);
                for (int i = 0; i < arrSize; i ++) {
                    printf("%s ", allStrings[i]);
                    free(allStrings[i]);
                }
                printf("\n");

                free(allStrings);

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

    printTree(root);

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

int isLeaf(BST_Node* node) {
    return (node->left == NULL && node->right == NULL);
}

int hasOnlyLeftChild(BST_Node* node) {
    return (node->left != NULL && node->right == NULL);
}

int hasOnlyRightChild(BST_Node* node) {
    return (node->left == NULL && node->right != NULL);
}

BST_Node* minVal(BST_Node* root) {

    // Root stores the minimal value.
    if (root->left == NULL)
        return root;

    // The left subtree of the root stores the minimal value.
    else
        return minVal(root->left);
}

BST_Node* removeFromGame(BST_Node* root, int game, char* word) {
    // Nothing happens in this case.
    if (root == NULL) return NULL;

    // Find the node to delete
    BST_Node* node = find(root, word);

    // Double check this exists
    if (!node) return root;

    // Remove word from the game
    node->ptr->allowed[game] = 0;

    // Word is in more than one game
    for (int i = 0; i < NUMGAMES; i++) {
        if (node->ptr->allowed[i] == 1) return root; // Stop
    }

    // This was the last game that had this word (delete the node using function from notes)
    return deleteNode(root, word);
}

BST_Node* deleteNode(BST_Node* root, char* word) {
    // delRec function from notes (modified to us strings)

    // Nothing happens in this case.
    if (root == NULL) return NULL;

    // Base case - found value to delete.
    if (strcmp(root->ptr->str, word) == 0) {
        // Leaf node.
        if (isLeaf(root)) {
            freeNode(root);
            return NULL;
        }

        // Has only a left child.
        else if (hasOnlyLeftChild(root)) {
            BST_Node* ret = root->left;
            freeNode(root);
            return ret;
        }

        // Has only a right child.
        else if (hasOnlyRightChild(root)) {
            BST_Node* ret = root->right;
            freeNode(root);
            return ret;
        }

        // two child case.
        else {
            BST_Node* new_del_node = minVal(root->right);
            NYT_String* save_ptr = root->ptr;
            root->ptr = new_del_node->ptr;
            new_del_node->ptr = save_ptr;
            root->right = deleteNode(root->right, new_del_node->ptr->str);
            return root;
        }
    }

    // Here we must go left.
    else if (strcmp(word, root->ptr->str) < 0)
        root->left = deleteNode(root->left, word);

    // And here we must go right.
    else
        root->right = deleteNode(root->right, word);

    // If we get here, this is the root of the tree.
    return root;
}

void freeNode(BST_Node* root) {
    free(root->ptr->str);
    free(root->ptr);
    free(root);
}

// Operation functions
BST_Node* addWord(BST_Node* root, int game, char* word) {
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
    // Just a wrapper function
    return removeFromGame(root, game, word);
}

void whichGame(BST_Node* root, char* word) {
    // Find the node for this word
    BST_Node* node = find(root, word);

    // Double check this exists
    if (!node) {
        printf("-1");
        return;
    }

    // Check and print which games it's in
    for (int i = 0; i < NUMGAMES; i++) {
        if (node->ptr->allowed[i] == 1) printf("%d ", i);
    }
    printf("\n");
}

char** allStringsInGame(BST_Node* root, int gameNo, int* arrSize) {
    /* For queries of type 4, while you could just run an inorder traversal of the tree and print as necessary, for
     * full credit, you will be required to write a function that takes in a pointer to the root of the binary search
     * tree, an integer indicating the game number, and a pointer to an integer which will store the length of the
     * array returned. The function should return a char**, an array of strings storing the answers for the query in
     * alphabetical order.
     *
     * To implement this function, please initially allocate a char** of size 200,000, or the size of the number of
     * nodes in the tree, copying in each string into this char** (using a deep copy so malloc just the right amount
     * of space for each string and then copy it in). Then, before returning, realloc the char** so that it's the right
     * size */

    if (root == NULL) {
        *arrSize = 0;
        return NULL; // no tree
    }

    int size = 0;
    BST_Node* curr = root;

    char** allStrings = malloc(sizeof(char*) * 200000);
    for (int i = 0; i < 200000; i++) {
        int len = strlen(curr->ptr->str);

        // Add string to array if in this game
        if (curr->ptr->allowed[gameNo] == 1) {
            allStrings[i] = malloc(sizeof(char) * (len+1));
            strcpy(allStrings[i], curr->ptr->str);
            size++; // increase size
        }

        // Move on to next node
        if (curr != NULL) curr = curr->left;
        else curr = curr->right;
    }

    allStrings = realloc(allStrings, sizeof(char*) * size);

    *arrSize = size; // size of the return array
    return allStrings;
}

void sameLengthWords(int game, int length) {
    printf("same length...\n");
}

void nextWord(BST_Node* root) {

}

// Debug
void printTree(BST_Node* root) {
    if (root == NULL) { printf("\n"); return; }
    printTree(root->left);
    printf("%s ", root->ptr->str);
    printTree(root->right);
}

void printGames(BST_Node* root) {
    if (root == NULL) return;
    for (int i = 0; i < NUMGAMES; i++) {
        printf("%d ", root->ptr->allowed[i]);
    }
    printf("\n");
}