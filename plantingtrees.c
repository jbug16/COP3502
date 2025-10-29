//
// Created by Jenna Curls on 10/23/25.
//

#include <stdio.h>
#include <stdlib.h>

// Function definitions
void printList(int* numbers, int length);
int isSorted(int values[], int low, int high);
void swap(int *a, int *b);
int partition(int* vals, int low, int high);
void quicksort(int* numbers, int low, int high);

int main() {
	// Get input
    int seeds;
    scanf("%d", &seeds);

    int* daysPerSeed = malloc(sizeof(int) * seeds);
    for (int i = 0; i < seeds; i++) {
        scanf("%d", &daysPerSeed[i]);
    }

	// Sort!!
	quicksort(daysPerSeed, 0, seeds - 1);

	// Determine when they finish growing
	int max = 0;
	for (int i = 0; i < seeds; i++) {
		// Get the day that the seed will finish growing
		// (Getting the last index because qsort is in ascending order
		int lastIndex = seeds - 1;
		int dayFinished = (i+1) + daysPerSeed[lastIndex - i];

		// Check if this is the longest time of all the tree growing days
		if (dayFinished > max) max = dayFinished;
	}
	max++; // "next" day because bro is lazy >:(

	printf("%d\n", max);

    // Clean up
    free(daysPerSeed);
}

// Using quicksort from class notes

void printList(int* numbers, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
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

int partition(int* vals, int low, int high) {
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