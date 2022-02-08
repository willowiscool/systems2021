#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h> 
#include <unistd.h> 

static void handle(int sig) {
	if (sig == SIGINT) {
		printf("Exiting due to SIGINT\n");
		exit(0);
	} else if (sig == SIGUSR1) {
		printf("Parent process PID: %d\n", getppid());
	}
}

int main() {
	signal(SIGINT, handle);
	signal(SIGUSR1, handle);
	while(1) {
		printf("Still running with pid %d\n", getpid());
		sleep(1);
	}
}
