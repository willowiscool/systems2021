#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

char** parse_args(char* line) {
	// get number of arguments
	int num = 1;
	int i;
	for (i = 0; i < strlen(line); i++)
		if (line[i] == ' ')
			num++;

	char** args = malloc(sizeof(char*) * (num + 1));
	// array must be null-terminated
	args[num] = NULL;
	char* linecpy = malloc(sizeof(line));
	strcpy(linecpy, line);
	for (i = 0; i < num; i++) {
		char* arg = strsep(&linecpy, " ");
		args[i] = arg;
	}

	return args;
}

int main() {
	printf("Args of `ls -l -a`:\n");
	char** args = parse_args("ls -l -a");
	printf("\targs[0]: %s\n", args[0]);
	printf("\targs[1]: %s\n", args[1]);
	printf("\targs[2]: %s\n", args[2]);
	printf("Calling\n");
	int err = execvp(args[0], args);
	if (err != 0) {
		printf("Error calling: %s\n", strerror(errno));
		return err;
	}
	return 0;
}
