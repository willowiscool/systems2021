//#include "client.h"
#include "types.h"
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
	printf("Guesses: %d\n", guesses);
	if (guesses == 0) printf("The word was %s.\n", msg.word);

	struct stat statStat;
	err = stat("./client_stats.bin", &statStat);
	struct stats stats;
	int statsFd = open("./client_stats.bin", O_RDWR | O_CREAT, 0644);
	if (err == 0) {
		read(statsFd, &stats, sizeof(stats));
		stats.played++;
		if (guesses == 0) {
			stats.streak = 0;
		} else {
			stats.streak++;
			stats.won++;
			if (stats.streak >= stats.maxStreak) stats.maxStreak = stats.streak;
			stats.guessDistribution[6 - guesses]++;
		}
	} else {
		// cba to loop
		stats.guessDistribution[0] = 0;
		stats.guessDistribution[1] = 0;
		stats.guessDistribution[2] = 0;
		stats.guessDistribution[3] = 0;
		stats.guessDistribution[4] = 0;
		stats.guessDistribution[5] = 0;

		stats.played = 1;
		if (guesses == 0) {
			stats.streak = 0;
			stats.maxStreak = 0;
			stats.won = 0;
		} else {
			stats.streak = 1;
			stats.maxStreak = 1;
			stats.won = 1;
			stats.guessDistribution[6 - guesses]++;
		}
	}
	lseek(statsFd, 0, SEEK_SET);
	write(statsFd, &stats, sizeof(stats));

	printf("\nStatistics:\n");
	// TODO make fancy guess distribution
	printf("Guess distribution: \n");
	printf("\t1 guess:\t%d\n", stats.guessDistribution[0]);
	printf("\t2 guesses:\t%d\n", stats.guessDistribution[1]);
	printf("\t3 guesses:\t%d\n", stats.guessDistribution[2]);
	printf("\t4 guesses:\t%d\n", stats.guessDistribution[3]);
	printf("\t5 guesses:\t%d\n", stats.guessDistribution[4]);
	printf("\t6 guesses:\t%d\n", stats.guessDistribution[5]);
	printf("Number of times played: %d\n", stats.played);
	printf("Percent won: %.2f%%\n", (float)stats.won / stats.played * 100);
	printf("Streak: %d\n", stats.streak);
	printf("Max streak: %d\n", stats.maxStreak);

	// send to server

	msg.guesses = 6 - guesses; // zero-indexed
	msg.stats = stats;
	write(sd, &msg, sizeof(msg));

	// get total stats from server
	read(sd, &msg, sizeof(msg));
	printf("\nGame-wide statistics:\n");
	printf("Guess distribution: \n");
	printf("\t1 guess:\t%d\n", msg.stats.guessDistribution[0]);
	printf("\t2 guesses:\t%d\n", msg.stats.guessDistribution[1]);
	printf("\t3 guesses:\t%d\n", msg.stats.guessDistribution[2]);
	printf("\t4 guesses:\t%d\n", msg.stats.guessDistribution[3]);
	printf("\t5 guesses:\t%d\n", msg.stats.guessDistribution[4]);
	printf("\t6 guesses:\t%d\n", msg.stats.guessDistribution[5]);
	printf("Number of times played: %d\n", msg.stats.played);
	printf("Percent won: %.2f%%\n", (float)msg.stats.won / msg.stats.played * 100);
	printf("Max streak: %d\n", msg.stats.maxStreak);
}
