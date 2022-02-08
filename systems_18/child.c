#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

int main() {
	int pipeToChild = open("pipeToChild", O_RDONLY, 0);
	int pipeToParent = open("pipeToParent", O_WRONLY, 0);

	char input[10000];
	int b = -1;
	while (b != 0) {
		b = read(pipeToChild, input, sizeof(input));
		// processing
		int i;
		for (i = 0; i < strlen(input); i++) {
			if (input[i] >= 97 && input[i] <= 122) input[i] -= 32;
		}
		write(pipeToParent, input, strlen(input));
	}
}
