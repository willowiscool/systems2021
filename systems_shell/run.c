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

FILE* runPopen(struct token* input, char* type) {
	// reconstruct command
	// assume input type is COMMAND
	int size = 0;
	char** end = input->command;
	while (*end != NULL) {
		size += strlen(*end) + 1;
		end++;
	}
	char command[size];
	char* ptr = command;
	char** arg = input->command;
	while (*arg != NULL) {
		strcpy(ptr, *arg);
		ptr += strlen(*arg);
		*ptr = ' ';
		ptr++;
		arg++;
	}
	*(ptr - 1) = '\0';

	return popen(command, type);
}

int runPipe(struct token* input) {
	// for now assume only two pipe commands
	// TODO: more pipes
	FILE* readFrom = runPopen(input->children[0], "r");
	FILE* writeTo = runPopen(input->children[1], "w");
	int fd[2];
	fd[0] = fileno(readFrom);
	fd[1] = fileno(writeTo);
	pipe(fd);
	pclose(readFrom);
	pclose(writeTo);
	return 0; // TODO TODO TODO
}

struct stdinAndStdoutFDs createRedirects(struct token* input) {
	struct stdinAndStdoutFDs sasfd;
	sasfd.stdin = STDIN_FILENO;
	sasfd.stdout = STDOUT_FILENO;
	int err;
	if (input->redirectTo != NULL) {
		sasfd.stdout = dup(STDOUT_FILENO);
		err = dup2(open(input->redirectTo, O_CREAT | O_WRONLY, 0644), STDOUT_FILENO);
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

// not currently used, couldn't make it work and had no idea why
// anyway delete this comment if you use it LMAO
int undoRedirects(struct stdinAndStdoutFDs sasfd) {
	int ret = 0;
	//if (sasfd.stdin != -1) ret = dup2(STDIN_FILENO, sasfd.stdin);
	//if (sasfd.stdout != -1) ret = dup2(STDOUT_FILENO, sasfd.stdout);
	ret = dup2(STDIN_FILENO, sasfd.stdin);
	ret = dup2(STDOUT_FILENO, sasfd.stdout);
	printf("hi\n");
	return ret;
}

int run(struct token* input) {
	if (input == NULL) return 0;
	if (input->type == SEMICOLON) {
		run(input->children[0]);
		return run(input->children[1]);
	}

	if (input->type == PIPE) {
		int ret = runPipe(input);
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
		// strip beginning whitespace
		char* command = input->command[0];
		while (*command == ' ' || *command == '\t') command++;
		execvp(command, input->command);
		createRedirects(input);
		// something happened
		printf("Error running %s: %s\n", input->command[0], strerror(errno));
		exit(1);
	} else {
		int status;
		waitpid(pid, &status, 0);
		return WEXITSTATUS(status);
	}
}
