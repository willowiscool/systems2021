#include "token.h"
#include <stdlib.h>
#include <string.h>

struct token* parseInput(char* input) {
	if (input == NULL || *input == '\0') return NULL;
	struct token* rootToken = malloc(sizeof(struct token));
	// check for semicolons
	char* semi = input;
	char* first = strsep(&semi, ";");
	if (semi != NULL) {
		// found a semicolon!
		rootToken->type = SEMICOLON;
		rootToken->children = malloc(sizeof(struct token*) * 3);
		rootToken->children[0] = parseInput(first);
		rootToken->children[1] = parseInput(semi);
		rootToken->children[2] = NULL;
		return rootToken;
	}

	rootToken->type = COMMAND;
	rootToken->beginningOfStrPtr = input;

	// strip spaces from the beginning
	while (*input == ' ' || *input == '\t') input++;
	// strip spaces from the end
	char* end = input;
	while (*end != '\0') end++;
	// todo this probably is gonna cause a segfault eventually
	end--;
	while (*end == ' ' || *end == '\t') end--;
	end++;
	*end = '\0';

	// count spaces
	int spaces = 0;
	char* incount = input;
	while (*incount != '\0') {
		if (*(incount++) == ' ') {
			spaces++;
			while (*(incount++) == ' ') incount++;
		}
	}

	// one more element than there are spaces
	spaces++;
	char** spaced = malloc(sizeof(char*) * (spaces + 1));
	int i;
	for (i = 0; i < spaces; i++)
		spaced[i] = strsep(&input, " ");

	spaced[i] = NULL;

	rootToken->command = spaced;

	return rootToken;

	/*struct token* currToken = rootToken;
	while (input != NULL) {
		currToken->type = TEXT;
		strcpy(currToken->token, strsep(&input, " "));
		currToken->children = malloc(sizeof(struct token*));
		struct token* nextToken = malloc(sizeof(struct token));
		currToken->children = &nextToken;
		currToken = nextToken;
	}

	return rootToken;*/
}
