#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct numberName {
	char* str;
	int i;
};

void printNumberName(struct numberName *nn) {
	printf("%d has the name %s\n", nn->i, nn->str);
}

struct numberName* createNumberName(char* name, int number) {
	struct numberName* nn = malloc(sizeof(struct numberName));
	nn->str = malloc(strlen(name));
	strcpy(nn->str, name);
	nn->i = number;
}

int main() {
	srand(time(NULL));
	struct numberName* nn;
	if (rand() > RAND_MAX / 2) {
		nn = createNumberName("Five", 5);
	} else {
		nn = createNumberName("Seven", 7);
	}
	printNumberName(nn);
}
