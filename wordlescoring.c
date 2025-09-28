//
// Created by Jenna Curls on 8/21/25.
//

#include <ctype.h>
#include <stdio.h>

// Constants
#define ALPHASIZE 26
#define MAXINPUT 1000
#define MAXWORDSIZE 5

// Function prototypes
void readInput(char words[][MAXWORDSIZE+1], int num);
void countLetters(char words[][MAXWORDSIZE+1], int num, int freq[ALPHASIZE]);
int scoreWord(char word[MAXWORDSIZE+1], int freq[ALPHASIZE]);

int main()
{
    // Get user input for number of words they will enter
    int num;
    scanf("%d", &num);

    // Static storage
    char words[MAXINPUT][MAXWORDSIZE + 1]; // Creates a 2D array with MAXINPUT being the index of the word and MAXWORDSIZE + 1 being the index of the character per word
    int freq[ALPHASIZE] = {0}; // Creates an array representing each letter and initializes each frequency to 0

    readInput(words, num);
    countLetters(words, num, freq);

    // Print output
    for (int i = 0; i < num; i++)
    {
        int score = scoreWord(words[i], freq);
        printf("%s\t%d\n", words[i], score);
    }

    return 0;
}

// Helper functions
void readInput(char words[][MAXWORDSIZE+1], int num)
{
    for (int i = 0; i < num; i++)
    {
        // Read only the first 5 letters and add this word to the current index
        scanf("%5s", words[i]);
    }
}

void countLetters(char words[][MAXWORDSIZE+1], int num, int freq[ALPHASIZE])
{
    // For each word
    for (int i = 0; i < num; i++)
    {
        // For each character
        for (int j = 0; j < MAXWORDSIZE; j++)
        {
            // Count how many letters there are in the input
            char character = words[i][j];
            // Make sure it's lowercase!!!!!
            freq[tolower(character) - 'a']++; // converts the ASCII form to an index 0-25 for each letter
        }
    }
}

// Returns the score for a single word
int scoreWord(char word[MAXWORDSIZE+1], int freq[ALPHASIZE])
{
    int sum = 0;

    // Loop through each character and add up the values based on the frequency
    for (int i = 0; i < MAXWORDSIZE; i++)
    {
        sum += freq[tolower(word[i]) - 'a']; // Make sure it's lowercase!!!
    }

    return sum;
}