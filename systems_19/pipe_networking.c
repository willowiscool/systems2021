#include "pipe_networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*=========================
	server_handshake
	args: int * to_client

	Performs the client side pipe 3 way handshake.
	Sets *to_client to the file descriptor to the downstream pipe.

	returns the file descriptor for the upstream pipe.
	=========================*/
int server_handshake(int *to_client) {
	int from_client = 0;

	int err = 0;
	err = mkfifo("serverpipe", 0644);
	if (err != 0) goto error;
	from_client = open("serverpipe", O_RDONLY, 0);
	if (from_client == -1) goto error;
	printf("Created and opened named pipe\n");

	char buff[1000];
	err = read(from_client, buff, sizeof(buff));
	if (err == -1) goto error;
	err = remove("serverpipe");
	if (err == -1) goto error;
	printf("Read name of client pipe and removed pipe created\n");

	*to_client = open(buff, O_WRONLY, 0);
	err = write(*to_client, "syn_ack", 8);
	if (err == -1) goto error;
	printf("Sent acknowledgement\n");

	err = read(from_client, buff, sizeof(buff));
	if (err == -1) goto error;
	if (strcmp(buff, "ack") != 0) {
		printf("Got wrong acknowledgement from client\n");
		return -1;
	}
	printf("Got acknowledgement from client\n");

	return from_client;

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
	int from_server = 0;

	char pidbuff[1000];
	sprintf(pidbuff, "%d", getpid());
	int err = 0;
	err = mkfifo(pidbuff, 0644);
	if (err != 0) goto error;
	printf("Created private pipe\n");

	*to_server = open("serverpipe", O_WRONLY, 0);
	if (*to_server == -1) goto error;
	err = write(*to_server, pidbuff, strlen(pidbuff) + 1);
	if (err == -1) goto error;
	from_server = open(pidbuff, O_RDONLY, 0);
	if (from_server == -1) goto error;
	printf("Connected to server pipe and sent name of private pipe. THEN opened private pipe\n");

	char buff[1000];
	err = read(from_server, buff, sizeof(buff));
	if (err == -1) goto error;
	if (strcmp(buff, "syn_ack") != 0) {
		printf("Got wrong acknowledgement from server\n");
		return -1;
	}
	err = remove(pidbuff);
	if (err == -1) goto error;
	printf("Read acknowledgement from server\n");

	err = write(*to_server, "ack", 4);
	if (err == -1) goto error;
	printf("Sent acknowledgement to server\n");

	return from_server;

	error:
	printf("Error: %s\n", strerror(errno));
	return -1;
}
