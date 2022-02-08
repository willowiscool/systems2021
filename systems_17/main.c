#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define READ 0
#define WRITE 1

int main() {
	int pipeToChild[2];
	int pipeToParent[2];
	pipe(pipeToChild);
	pipe(pipeToParent);

	int pid = fork();

	if (pid == -1) {
		printf("Problem forking: %s\n", strerror(errno));
		return 1;
	}

	if (pid == 0) {
		// child
		close(pipeToChild[WRITE]);
		close(pipeToParent[READ]);

		char input[10000];
		int b = -1;
		while (b != 0) {
			b = read(pipeToChild[READ], input, sizeof(input));
			// processing
			int i;
			for (i = 0; i < strlen(input); i++) {
				if (input[i] >= 97 && input[i] <= 122) input[i] -= 32;
			}
			write(pipeToParent[WRITE], input, strlen(input));
		}
	} else {
		// parent
		close(pipeToParent[WRITE]);
		close(pipeToChild[READ]);

		char input[10000];
		while (1) {
			printf("Input: ");
			// detect when EOF is sent by making the default string empty
			input[0] = '\0';
			fgets(input, sizeof(input), stdin);
			if (strlen(input) == 0) return 0;
			write(pipeToChild[WRITE], input, sizeof(input));
			read(pipeToParent[READ], input, sizeof(input));
			printf("Returned from child: %s", input);
		}
	}
}
