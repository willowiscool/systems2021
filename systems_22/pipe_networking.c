#include "pipe_networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*=========================
	server_handshake
	args: int * to_client

	Performs the client side pipe 3 way handshake.
	Sets *to_client to the file descriptor to the downstream pipe.

	returns the file descriptor for the upstream pipe.
	=========================*/
int server_handshake(int *to_client) {
	struct addrinfo * hints, * results;
	hints = calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, "8080", hints, &results);
	printf("Got addrinfo\n");

	int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	if (sd == -1) goto error;
	bind(sd, results->ai_addr, results->ai_addrlen);
	listen(sd, 10);
	printf("Created socket and listening\n");

	free(hints);
	freeaddrinfo(results);
	while(1) {
		socklen_t sock_size;
		struct sockaddr_storage client_address;
		sock_size = sizeof(client_address);
		*to_client = accept(sd, (struct sockaddr*)&client_address, &sock_size);
		printf("Got client. Forking\n");
		int pid = fork();
		if (pid == -1) goto error;
		if (pid == 0) {
			// child
			return *to_client;
		}
	}
	error:
	printf("Error: %s\n", strerror(errno));
	return -1;
}


/*=========================
	client_handshake
	args: int * to_server

	Performs the client side pipe 3 way handshake.
	Sets *to_server to the file descriptor for the upstream pipe.

	returns the file descriptor for the downstream pipe.
	=========================*/
int client_handshake(int *to_server) {
	struct addrinfo * hints, * results;
	hints = calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, "8080", hints, &results);
	printf("Got addrinfo\n");

	int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	connect(sd, results->ai_addr, results->ai_addrlen);
	free(hints);
	freeaddrinfo(results);
	printf("Connected to server\n");

	*to_server = sd;
	return sd;
}
