#ifndef RUN_H
#define RUN_H
#include "token.h"
#include <stdio.h>

int run(struct token* input);
FILE* runPopen(struct token* input, char* type);
int runPipe(struct token* input, char* redirectTo);

struct stdinAndStdoutFDs {
	int stdin;
	int stdout;
};

struct stdinAndStdoutFDs createRedirects(struct token* input);
int undoRedirects(struct stdinAndStdoutFDs sasfd);
#endif // RUN_H
