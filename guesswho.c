#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function definitions
void freeCharacters(char** characters, int characterCount);

int main() {
    // n is how many characters
    // m is how many attributes each character has
    // q is how many questions/answers for the hidden character
    int n, m, q;

    // get input for number of characters, attributes and questions
    scanf("%d %d %d", &n, &m, &q);

    // allocate the memory
    char** characters = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        characters[i] = malloc((m+1) * sizeof(char));
    }

    // get input for each character attribute
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf(" %c", &characters[i][j]);
        }
        characters[i][m] = '\0';
    }

    // get input for queries
    char* hidden = malloc((m+1) * sizeof(char));
    int idx;
    char ans;

    // init hidden attributes
    for (int i = 0; i < m; i++) {
        hidden[i] = '?';
    }

    for (int i = 0; i < q; i++) {
        scanf("%d %c", &idx, &ans);
        hidden[idx-1] = ans;
    }

    // check if the hidden character can easily be found

    int count = 0;
    int lastIdx = -1;

    // loop through all the characters
    for (int i = 0; i < n; i ++) {
        // the number of similar attributes
        int same = 0;

        // loop through each attribute
        for (int j = 0; j < m; j++) {
            if (hidden[j] == '?') continue;
            if (characters[i][j] == hidden[j]) {
                same++;
            }
        }

        // check if there is the same amount of queries and same attributes
        if (same == q) {
            count++;
            lastIdx = i;
        }
    }

    // check if there are more than 1 of the same potential characters
    if (count == 1) {
        printf("unique\n%d\n", lastIdx + 1);
    } else if (count > 1) {
        printf("ambiguous\n%d\n", count);
    }

    // free the memory
    freeCharacters(characters, n);
    free(hidden);

    return 0;
}

void freeCharacters(char** characters, int characterCount) {
    if (characters == NULL) return;

    for (int i = 0; i < characterCount; i++) {
        free(characters[i]);
    }
    free(characters);
}