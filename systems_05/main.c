#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	srand(time(NULL));

	int arr[10];
	arr[0] = 0;
	int i;
	// could do these in one loop but they were in separate
	// instructions so I decided to do it in separate loops
	for (i = 1; i < 10; i++) arr[i] = rand();
	for (i = 0; i < 10; i++) printf("%d ", arr[i]);
	printf("\n");

	int arr2[10];
	int *parr = arr;
	int *parr2 = arr2;
	// reverse order populate
	for (i = 0; i < 10; i++) parr2[i] = parr[9 - i];
	// print out values in second array
	for (i = 0; i < 10; i++) printf("%d ", *(parr2 + i));
	printf("\n");
}
