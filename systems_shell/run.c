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
#include <fcntl.h>

// not using popen so we can pipe more than 2 commands
int runPipe(struct token* input, char* redirectTo) {
	char* redirectFrom = input->redirectFrom;

	int pid = fork();
	if (pid == 0) {
		// child
		int filedes[2];
		pipe(filedes);

		int pid2 = fork();
		if (pid2 == 0) {
			dup2(filedes[1], STDOUT_FILENO);
			close(filedes[0]);

			if (redirectFrom != NULL) {
				dup2(open(redirectFrom, O_RDONLY, 0), STDIN_FILENO);
			}

			input = input->children[0];
			// strip beginning whitespace
			char* command = input->command[0];
			while (*command == ' ' || *command == '\t') command++;
			execvp(command, input->command);
			// something happened
			printf("Error running %s: %s\n", input->command[0], strerror(errno));
			exit(1);
		} else {
			dup2(filedes[0], STDIN_FILENO);
			close(filedes[1]);

			if (input->children[1]->type == PIPE) {
				runPipe(input->children[1], redirectTo);
			} else {
				if (redirectTo != NULL) {
					dup2(open(redirectTo, O_CREAT | O_WRONLY | O_TRUNC, 0644), STDOUT_FILENO);
				}

				input = input->children[1];
				// strip beginning whitespace
				char* command = input->command[0];
				while (*command == ' ' || *command == '\t') command++;
				execvp(command, input->command);
				// something happened
				printf("Error running %s: %s\n", input->command[0], strerror(errno));
				exit(1);
			}
		}
	} else {
		int status;
		waitpid(pid, &status, 0);
		return WEXITSTATUS(status);
	}
}

struct stdinAndStdoutFDs createRedirects(struct token* input) {
	struct stdinAndStdoutFDs sasfd;
	sasfd.stdin = STDIN_FILENO;
	sasfd.stdout = STDOUT_FILENO;
	int err;
	if (input->redirectTo != NULL) {
		int flags = O_CREAT | O_WRONLY;
		if (input->append) flags |= O_APPEND;
		else flags |= O_TRUNC;
		sasfd.stdout = dup(STDOUT_FILENO);
		err = dup2(open(input->redirectTo, flags, 0644), STDOUT_FILENO);
		if (err == -1) {
			printf("Error redirecting to %s: %s\n", input->redirectTo, strerror(errno));
		}
	}
	if (input->redirectFrom != NULL) {
		sasfd.stdin = dup(STDIN_FILENO);
		err = dup2(open(input->redirectFrom, O_RDONLY), STDIN_FILENO);
		if (err == -1) {
			printf("Error redirecting from %s: %s\n", input->redirectFrom, strerror(errno));
		}
	}
	return sasfd;
}

// Not used anywhere right now
int undoRedirects(struct stdinAndStdoutFDs sasfd) {
	int ret = 0;
	if (sasfd.stdin != STDIN_FILENO) close(STDIN_FILENO);
	ret = dup2(sasfd.stdin, STDIN_FILENO);
	if (sasfd.stdout != STDOUT_FILENO) close(STDOUT_FILENO);
	ret = dup2(sasfd.stdout, STDOUT_FILENO);
	return ret;
}

int run(struct token* input) {
	if (input == NULL) return 0;
	if (input->type == SEMICOLON) {
		run(input->children[0]);
		return run(input->children[1]);
	}

	if (input->type == PIPE) {
		int ret = runPipe(input, input->redirectTo);
		return ret;
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
		createRedirects(input);
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
