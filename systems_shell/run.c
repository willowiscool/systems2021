#include "run.h"
#include "token.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int run(struct token* input) {
	if (input == NULL) return 0;
	if (input->type == SEMICOLON) {
		run(input->children[0]);
		return run(input->children[1]);
	}

	// input->type assumed to be COMMAND (in case you add more and forget to return)
	if (input->command[0] == NULL || strcmp(input->command[0], "") == 0) return 0;

	if (strcmp(input->command[0], "cd") == 0) {
		int ret;
		if (input->command[1] == NULL) {
			// thanks stackoverflow
			char* homedir = getenv("HOME");
			if (homedir == NULL) {
				homedir = getpwuid(getuid())->pw_dir;
			}
			ret = chdir(homedir);
		} else {
			ret = chdir(input->command[1]);
		}
		if (ret != 0) {
			printf("Error changing directory: %s\n", strerror(errno));
			return ret;
		}
		return 0;
	}

	int pid = fork();
	if (pid == 0) {
		// child
		// strip beginning whitespace
		char* command = input->command[0];
		while (*command == ' ' || *command == '\t') command++;
		execvp(command, input->command);
		// something happened
		printf("Error running %s: %s\n", input->command[0], strerror(errno));
		exit(1);
	} else {
		int status;
		waitpid(pid, &status, 0);
		return WEXITSTATUS(status);
	}
}
