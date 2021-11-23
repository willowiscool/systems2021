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

			return innerToken;
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

			return innerToken;
		}
	}

	if (rootToken->type != SEMICOLON) {
		second = input;
		first = strsep(&second, "|");
		if (second != NULL) rootToken->type = PIPE;
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

void printToken(struct token* token, int level) {
	// can break lol don't make things too big
	char str[10000];
	if (token->type == SEMICOLON) {
		printTabs(level, "TOKEN TYPE: SEMICOLON\n");
		printToken(token->children[0], level + 1);
		printToken(token->children[1], level + 1);
	} else if (token->type == PIPE) {
		printTabs(level, "TOKEN TYPE: PIPE\n");
		sprintf(str, "REDIRECT TO: %s\n", token->redirectTo);
		if (token->redirectTo != NULL) printTabs(level, str);
		sprintf(str, "REDIRECT FROM: %s\n", token->redirectFrom);
		if (token->redirectFrom != NULL) printTabs(level, str);
		printToken(token->children[0], level + 1);
		printToken(token->children[1], level + 1);
	} else {
		printTabs(level, "TOKEN TYPE: COMMAND\n");
		printTabs(level, "COMMAND: ");
		int i = 0;
		while (token->command[i] != NULL) printf("%s ", token->command[(i++)]);
		printf("\n");
		sprintf(str, "REDIRECT TO: %s\n", token->redirectTo);
		if (token->redirectTo != NULL) printTabs(level, str);
		sprintf(str, "REDIRECT FROM: %s\n", token->redirectFrom);
		if (token->redirectFrom != NULL) printTabs(level, str);
	}
}

void printTabs(int level, char* str) {
	while (level > 0) {
		printf("\t");
		level--;
	}
	printf(str);
}
