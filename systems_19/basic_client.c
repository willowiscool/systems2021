#include "pipe_networking.h"


int main() {

	int to_server;
	int from_server;

	from_server = client_handshake( &to_server );

	printf("Reading test message\n");
	char hello[6];
	read(from_server, hello, 6);
	printf(hello);
	printf("Sending test message\n");
	write(to_server, "hi", 3);
}
