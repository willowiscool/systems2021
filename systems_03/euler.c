#include "euler.h"

// Sum of all multiples of 3 or 5 under 1000
int pe1() {
	int sum = 0;
	int count;
	for (count = 0; count < 1000; count++) {
		if (count % 3 == 0) sum += count;
		if (count % 5 == 0) sum += count;
		if (count % 15 == 0) sum -= count;
	}
	return sum;
}

// Smallest multiple of all of the numbers 1..20
int pe5() {
	// could probably optimize by choosing a high starting number
	int number = 0;
	int divisible = 0;
	while (!divisible) {
		number += 20;
		divisible = 1;
		int divisor;
		// could probably optimise by not checking 2, 4, 5, 10
		for (divisor = 1; divisor <= 20; divisor++) {
			if (number % divisor != 0) divisible = 0;
		}
	}
	return number;
}

// Difference between the sum of the squares of the first 100 natural numbers and the square of the sum
int pe6() {
	int sum_of_squares = 0;
	int sum = 0;
	int n;
	for (n = 1; n <= 100; n++) {
		sum_of_squares += n * n;
		sum += n;
	}
	return (sum * sum) - sum_of_squares;
}
