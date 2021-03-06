#include "server.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
	srand(time(NULL));

	time_t timeGenerated;
	char* word = getWord(&timeGenerated);

	if (argc > 1 && strcmp(argv[1], "-show") == 0) printf("Daily word: %s\n", word);
	printf("Word generated: %s\n", ctime(&timeGenerated));

	// Get addrinfo
	struct addrinfo* hints, * results;
	hints = calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, "8080", hints, &results);

	int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	if (sd == -1) {
		printf("Error creating socket: %s\n", strerror(errno));
		exit(1);
	}
	bind(sd, results->ai_addr, results->ai_addrlen);
	listen(sd, 10);

	free(hints);
	freeaddrinfo(results);

	// read and write from srvfd
	int srvfd;
	while (1) {
		socklen_t sock_size;
		struct sockaddr_storage client_address;
		sock_size = sizeof(client_address);
		srvfd = accept(sd, (struct sockaddr*)&client_address, &sock_size);
		int pid = fork();
		if (pid == -1) {
			printf("Error forking: %s\n", strerror(errno));
			exit(1);
		}
		if (pid == 0) break;
	}

	// send word
	struct message msg;
	strcpy(msg.word, word);
	msg.timeGenerated = timeGenerated;
	free(word);
	write(srvfd, &msg, sizeof(msg));

	// get stats
	// TODO consolidate reading stats into one func??
	read(srvfd, &msg, sizeof(msg));
	struct stats stats = writeStats("./server_stats.bin", msg.guesses, 1);

	// TODO daily statistics?
	// send back to client
	msg.stats = stats;
	write(srvfd, &msg, sizeof(msg));

	return 0;
}

char* getWord(time_t* timeGenerated) {
	struct stat dailyStat;
	int exists = stat("./daily_word.txt", &dailyStat);
	time_t now = time(NULL);
	// exists != 0 -> it doesn't exist
	// time_t is number of seconds since epoch
	if (exists != 0 || now - dailyStat.st_mtime > 24 * 60 * 60) {
		struct stat answerStat;
		int err = stat("./answer_words.txt", &answerStat);
		if (err != 0) {
			printf("Answer words file not found.\n");
			exit(1);
		}
		char* answerWords = malloc(answerStat.st_size);
		int answers = open("./answer_words.txt", O_RDONLY, 0);
		read(answers, answerWords, answerStat.st_size);
		close(answers);

		int numWords = 0;
		char* ptr = answerWords;
		// NOTE: assumes that first char is not a newline
		while (*(ptr++) != '\0')
			if (*ptr == '\n') numWords++;

		// NOTE: assumes five-letter words and a new line to separate them
		int index = (rand() % numWords) * 6;

		char* word = malloc(6);
		strncpy(word, (answerWords + index), 5);
		word[5] = '\0';
		*timeGenerated = time(NULL);

		free(answerWords);

		int daily = open("./daily_word.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);
		write(daily, word, 6);
		close(daily);
		return word;
	}
	int daily = open("./daily_word.txt", O_RDONLY, 0);
	*timeGenerated = dailyStat.st_mtime;
	char* word = malloc(dailyStat.st_size);
	read(daily, word, dailyStat.st_size);
	close(daily);
	if (word[dailyStat.st_size - 1] == '\n') word[dailyStat.st_size - 1] = '\0';
	return word;
}
