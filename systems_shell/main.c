#include "token.h"
#include "main.h"
#include "run.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printPrompt() {
	printf("$ ");
}

char* getInput() {
	// TODO: can we avoid 1000?
	char input[1000];
	char* beginning = fgets(input, 1000, stdin);
	if (beginning == NULL) {
		return NULL;
	}
	// strip beginning whitespace
	while (*beginning == '\t' || *beginning == ' ') {
		beginning++;
	}
	char* end = beginning;
	// get size, start at 1 for terminating null
	int size = 1;
	while (*end != '\n' && *end != '\0') {
		end++;
		size++;
	}
	*end = '\0';
	char* output = malloc(size);
	strcpy(output, beginning);
	return output;
}
int main() {
	while (1) {
		printPrompt();
		char* inputRaw = getInput();
		if (inputRaw == NULL) exit(0);
		if (strcmp(inputRaw, "exit") == 0) exit(0);
		struct token* input = parseInput(inputRaw);

		if (input->type == COMMAND) {
			if (strcmp(input->command[0], "exit") == 0) {
				if (input->command[1] != NULL) exit(atoi(input->command[1]));
				exit(0);
			}
			if (input->command[0] == NULL) continue;
			if (strcmp(input->command[0], "") == 0) continue;
		}

		int exit = run(input);
		// TODO FREE INPUT
	}
}
