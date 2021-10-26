#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int randomNumber() {
	int fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1) {
		printf("Error opening /dev/urandom: %s\n", strerror(errno));
		return -1;
	}
	int numberOfBytes = sizeof(int);
	int num = 0;
	int b = read(fd, &num, numberOfBytes);
	if (b == -1) {
		printf("Error reading from random: %s\n", strerror(errno));
		return -1;
	}
	return num;
}

int main() {
	int nums[10];
	int i;
	printf("Generating random numbers:\n");
	for (i = 0; i < 10; i++) {
		nums[i] = randomNumber();
		printf("\trandom %d: %d\n", i, nums[i]);
	}

	printf("\nWriting numbers to file...\n\n");
	int fd = open("random_numbers.bin", O_WRONLY | O_CREAT, 0644);
	if (fd == -1) {
		printf("Error opening/creating random_numbers.bin: %s\n", strerror(errno));
		return -1;
	}
	int b = write(fd, nums, sizeof(int) * 10);
	if (b == -1) {
		printf("Error writing numbers to file: %s\n", strerror(errno));
		return -1;
	}

	printf("Reading numbers from file...\n\n");
	int nums2[10];
	// breaks if I don't reset fd, probably something to ask about but 🤷
	fd = open("random_numbers.bin", O_RDONLY);
	if (fd == -1) {
		printf("Error opening random_numbers.bin: %s\n", strerror(errno));
		return -1;
	}
	b = read(fd, nums2, sizeof(int) * 10);
	if (b == -1) {
		printf("Error reading numbers from file: %s\n", strerror(errno));
		return -1;
	}

	printf("Verification that written values were the same:\n");
	for (i = 0; i < 10; i++) {
		printf("\trandom %d: %d\n", i, nums2[i]);
	}
}
