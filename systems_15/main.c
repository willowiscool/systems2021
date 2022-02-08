#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
	printf("Initial message\n");
	int f = fork();
	int f2;
	if (f) {
		f2 = fork();
	}
	if (f && f2) {
		int status;
		int pid = wait(&status);
		printf("%d finished. Took %d seconds\n", pid, WEXITSTATUS(status));
		return 0;
	}

	srand(getpid());
	int r = rand() % 4 + 2;
	sleep(r);
	printf("Finished sleeping for %d seconds\n", r);
	return r;
}
