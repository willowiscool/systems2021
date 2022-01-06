#include "pipe_networking.h"
#include <stdio.h>
#include <unistd.h>

int main() {

	int to_server;
	int from_server;

	from_server = client_handshake( &to_server );
	printf("Finished creating connection\n\n");

	char buff[1000];
	while (1) {
		printf("Send: ");
		buff[0] = '\0';
		fgets(buff, sizeof(buff), stdin);
		if (buff[0] == '\0') return 0;
		write(to_server, buff, sizeof(buff));
		int bytes = read(from_server, buff, sizeof(buff));
		if (bytes <= 0) {
			printf("Server exited\n");
			return 0;;
		}
		printf("Received: %s\n", buff);
	}
}
