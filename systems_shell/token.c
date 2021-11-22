#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct token* parseInput(char* input) {
	if (input == NULL || *input == '\0') return NULL;
	struct token* rootToken = malloc(sizeof(struct token));

	// check for splitting tokens
	// any token that splits rootToken into two
	rootToken->type = COMMAND;
	char* second = input;
	char* first = strsep(&second, ";");
	if (second != NULL) rootToken->type = SEMICOLON;
	else {
		second = input;
		first = strsep(&second, "|");
		if (second != NULL) rootToken->type = PIPE;
	}

	rootToken->redirectTo = NULL;
	rootToken->redirectFrom = NULL;

	if (rootToken->type != SEMICOLON) {
		char* afterGt = input;
		char* beforeGt = strsep(&afterGt, ">");
		if (afterGt != NULL) {
			free(rootToken);
			struct token* innerToken = parseInput(beforeGt);
			while (*afterGt != '\0' && (*afterGt == ' ' || *afterGt == '\t')) afterGt++;
			// TODO error rather than just ignoring it LOL
			if (*afterGt == '\0') return innerToken;
			// if you switch to not mallocing input turn this to strcpy!!!
			innerToken->redirectTo = afterGt;
		}
		// copy paste for the win
		char* afterLt = input;
		char* beforeLt = strsep(&afterLt, "<");
		if (afterLt != NULL) {
			free(rootToken);
			struct token* innerToken = parseInput(beforeLt);
			while (*afterLt != '\0' && (*afterLt == ' ' || *afterLt == '\t')) afterLt++;
			// TODO error rather than just ignoring it LOL
			if (*afterLt == '\0') return innerToken;
			// if you switch to not mallocing input turn this to strcpy!!!
			innerToken->redirectFrom = afterLt;
		}
	}

	if (rootToken->type == SEMICOLON || rootToken->type == PIPE) {
		rootToken->children = malloc(sizeof(struct token*) * 3);
		rootToken->children[0] = parseInput(first);
		rootToken->children[1] = parseInput(second);
		rootToken->children[2] = NULL;
		return rootToken;
	}

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
	for (i = 0; i < spaces; i++) {
		spaced[i] = strsep(&input, " ");
		// in case multiple spaces
		while (input != NULL && *input != '\0' && (*input == ' ' || *input == '\t')) input++;
	}

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
