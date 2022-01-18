//#include "client.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	// GET POSSIBLE WORDS
	struct stat posStat;
	int err = stat("./all_words.txt", &posStat);
	if (err != 0) {
		printf("Error reading possible words: %s\n", strerror(errno));
		return 1;
	};
	char* posWords = malloc(posStat.st_size);
	int pos = open("./all_words.txt", O_RDONLY, 0);
	read(pos, posWords, posStat.st_size);
	close(pos);

	// CONNECT TO SERVER
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

	// READ FROM SERVER
	struct message msg;
	read(sd, &msg, sizeof(msg));

	int guesses = 6;

	char guess[1000];
	while (guesses > 0) {
		printf("Guess: ");
		fgets(guess, 1000, stdin);
		int i;
		if (guess[5] == '\n') guess[5] = '\0';
		if (strlen(guess) != 5 || guess[4] == '\n') {
			printf("Guess must be 5 letters long!\n");
			continue;
		}

		// Check possible words
		char* posWord = posWords;
		int foundWord = 0;
		while (*posWord != '\0' && *posWord != '\n') {
			if (strncmp(posWord, guess, 5) == 0) foundWord = 1;
			posWord += 6;
		}
		if (!foundWord) {
			printf("Guess not in word list!\n");
			continue;
		}

		if (strcmp(guess, msg.word) == 0) {
			printf("\033[42m%s\033[m\n", guess);
			break;
		}
		char outOfPlaceLetters[6];
		strncpy(outOfPlaceLetters, msg.word, 6);
		for (i = 0; i < 5; i++) {
			if (guess[i] == msg.word[i]) {
				printf("\033[42m%c\033[m", guess[i]);
				outOfPlaceLetters[i] = '\0';
			} else {
				int printed = 0;
				int j;
				for (j = 0; j < 5; j++) {
					if (guess[i] == outOfPlaceLetters[j]) {
						printf("\033[43m%c\033[m", guess[i]);
						outOfPlaceLetters[j] = '\0';
						printed = 1;
					}
				}
				if (!printed) printf("%c", guess[i]);
			}
		}
		printf("\n");
		guesses--;
	}
	if (guesses == 0) printf("The word was %s. Better luck tomorrow!\n", msg.word);
}
