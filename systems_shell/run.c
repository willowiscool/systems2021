#include "run.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int run(char** input) {
	// TODO cd
	int pid = fork();
	if (pid == 0) {
		// child
		execvp(input[0], input);
		// something happened
		printf("Error running %s: %s\n", input[0], strerror(errno));
		exit(1);
	} else {
		int status;
		waitpid(pid, &status, 0);
		return WEXITSTATUS(status);
	}
}
