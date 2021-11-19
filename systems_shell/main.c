#include "main.h"
#include "run.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printPrompt() {
	printf("$ ");
}

// sometimes, fgets doesn't get the whole line in one go
// this function will add together all of the chunks until the newline
// hopefully the case doesn't happen where fgets gets something like abc\ndefg...
// don't see that happening any time soon
char* getInput() {
	char* input = malloc(1000);
	char* in = fgets(input, 1000, stdin);
	if (in == NULL) {
		free(input);
		return NULL;
	}
	while (*in != '\n' && *in != '\0') in++;
	*in = '\0';
	return input;
}
char** parseInput(char* input) {
	// count spaces
	int spaces = 0;
	char* incount = input;
	while (*incount != '\0')
		if (*(incount++) == ' ')
			spaces++;

	// one more element than there are spaces
	spaces++;
	char** spaced = malloc(sizeof(char*) * (spaces + 1));
	int i;
	for (i = 0; i < spaces; i++)
		spaced[i] = strsep(&input, " ");

	spaced[i] = NULL;

	return spaced;
}

int main() {
	while (1) {
		printPrompt();
		char* inputRaw = getInput();
		if (inputRaw == NULL) exit(0);
		if (strcmp(inputRaw, "exit") == 0) exit(0);
		char** input = parseInput(inputRaw);
		// TODO exit with the right code
		if (strcmp(input[0], "exit") == 0) {
			if (input[1] != NULL) exit(atoi(input[1]));
			exit(0);
		}
		if (input[0] == NULL) continue;
		if (strcmp(input[0], "") == 0) continue;

		int exit = run(input);
		// not using exit for now but can integrate into variables if I do that
		free(input[0]); // points to the start of the whole block
		free(input);
	}
}
