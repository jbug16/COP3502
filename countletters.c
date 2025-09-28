// 8/20/2025
// TODO: Finish writing this program as practice

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Use constants for numbers like these
#define ALPHASIZE 26
#define MAXSTRSIZE 10000

// Remember to write function prototypes
int* makeFreq(char* str);

int main()
{
    // Allocate space and read in a string
    char* word = malloc(sizeof(char) * MAXSTRSIZE);
    printf("Enter a string: ");
    scanf("%s", word);

    // Call function to get frequency of letters in word
    int* myf = makeFreq(word);
    
    // Print out freqs
    for (int i = 0; i < ALPHASIZE; i++)
        printf("%c\t%d\n", 'a'+i, myf[i]);

    // Free the space
    free(word);
    free(myf);
    
    return 0;
}

// Takes in a string and counts the lowercase letters in it.
// Stores those frequencies in an array and returns a pointer to that array.
int* makeFreq(char* str)
{
    // Defines the length of the input string
    int len = strlen(str);

    // Initialize the frequency array by setting each index to 0
    int* freq = malloc(sizeof(int) * ALPHASIZE); // Allocate memory per letter
    for (int i = 0; i < ALPHASIZE; i++)
    {
        freq[i] = 0;
    }

    // Go through all the letters
    for (int i = 0; i < len; i++) 
    {
        // Make sure it's lowercase
        if (str[i] >= 'a' && str[i] <= 'z')
            freq[str[i] - 'a']++; // converts the ASCII form to an index 0-25 for each letter
    }

    return freq;
}