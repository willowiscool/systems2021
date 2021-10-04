#include <stdio.h>

// header? I hardly new her!
void printIntArray(int arr[], int length) {
	int i;
	printf("[ ");
	for (i = 0; i < length; i++)
		printf("%d ", arr[i]);
	printf("]\n");
}
int avgOfIntArray(int arr[], int length) {
	int i, sum = 0;
	for (i = 0; i < length; i++)
		sum += arr[i];
	return sum / length; // didn't say what type to return...
}
void copyOneIntArrayIntoAnother(int arr[], int arr2[], int length) {
	int i;
	for (i = 0; i < length; i++)
		arr2[i] = arr[i];
}
int main() {
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	printf("Array: ");
	printIntArray(arr, 10);
	printf("Avg: %d\n", avgOfIntArray(arr, 10));
	int arr2[10];
	copyOneIntArrayIntoAnother(arr, arr2, 10);
	printf("Copied array: ");
	printIntArray(arr2, 10);
}
