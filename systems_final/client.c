//#include "client.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	struct addrinfo* hints, * results;
	hints = calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	if (argc > 1) getaddrinfo(argv[1], "8080", hints, &results);
	else getaddrinfo(NULL, "8080", hints, &results);

	int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	connect(sd, results->ai_addr, results->ai_addrlen);
	free(hints);
	freeaddrinfo(results);

	struct message msg;
	read(sd, &msg, sizeof(msg));

/*	char guess[7];
	printf("Guess: ");
	fgets(guess, 7, stdin);
	printf("Guess: [%s]\n", guess);*/
}
