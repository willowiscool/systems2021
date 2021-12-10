#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	mkfifo("pipeToChild", 0644);
	mkfifo("pipeToParent", 0644);

	int pipeToChild = open("pipeToChild", O_WRONLY, 0);
	int pipeToParent = open("pipeToParent", O_RDONLY, 0);

	char input[10000];
	while (1) {
		printf("Input: ");
		// detect when EOF is sent by making the default string empty
		input[0] = '\0';
		fgets(input, sizeof(input), stdin);
		if (strlen(input) == 0) return 0;
		write(pipeToChild, input, sizeof(input));
		read(pipeToParent, input, sizeof(input));
		printf("Returned from child: %s", input);
	}
}
