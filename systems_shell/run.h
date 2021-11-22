#ifndef RUN_H
#define RUN_H
#include "token.h"
#include <stdio.h>

int run(struct token* input);
FILE* runPopen(struct token* input, char* type);
int runPipe(struct token* input);
#endif // RUN_H
