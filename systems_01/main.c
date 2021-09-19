#include <stdio.h>

int fib(int a, int b, int n) {
	while (n > 0) {
		int temp = b;
		b = a + b;
		a = temp;
		n--;
	}
	return a;
}

int main() {
	printf("3rd fib: %d\n\n", fib(0, 1, 3));

	for (int n = 1; n <= 10; n++) {
		printf("fib no. %d: %d\n", n, fib(0, 1, n));
	}

	// prints the 0th char, then knows to stop when it wraps around
	printf("\nAll chars:\n%c", 0);
	for (char c = 1; c > 0; c++) {
		printf("%c", c);
	}

	return 0;
}

