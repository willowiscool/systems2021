#include "pipe_networking.h"
#include <unistd.h>
#include <signal.h>

static void sighandler(int signo) {
	if (signo == SIGINT) {
		printf("Exiting\n");
		exit(0);
	}
}

int main() {
	signal(SIGINT, sighandler);
	int to_client;
	int from_client;

	from_client = server_handshake( &to_client );
	printf("Finished creating connection\n\n");

	char buff[1000];
	while (1) {
		buff[0] = '\0';
		int bytes = read(from_client, buff, sizeof(buff));
		if (bytes == 0) {
			// eof
			break;
		}
		// process
		int i;
		for (i = 0; i < strlen(buff); i++)
			if (buff[i] >= 97 && buff[i] <= 122) buff[i] -= 32;

		write(to_client, buff, sizeof(buff));
	}
}
