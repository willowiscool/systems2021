//#include "client.h"
#include "types.h"
#include <curses.h>
#include <locale.h>
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

	// init ncurses, clear screen, and make board
	setlocale(LC_ALL, "UTF-8");
	initscr();
	start_color();
	// 0 is default, 1 is yellow, 2 is green
	init_pair(1, COLOR_WHITE, COLOR_YELLOW);
	init_pair(2, COLOR_WHITE, COLOR_GREEN);
	scrollok(stdscr, TRUE);
	cbreak();
	noecho();
	curs_set(0);
	clear();
	addstr("= W O R D L E =\n");
	int i;
	int j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 5; j++) {
			addch(ACS_ULCORNER);
			addch(ACS_HLINE);
			addch(ACS_URCORNER);
		}
		addch('\n');
		for (j = 0; j < 5; j++) {
			addch(ACS_VLINE);
			addch(' ');
			addch(ACS_VLINE);
		}
		addch('\n');
		for (j = 0; j < 5; j++) {
			addch(ACS_LLCORNER);
			addch(ACS_HLINE);
			addch(ACS_LRCORNER);
		}
		addch('\n');
		/* can't do this because curses is weird with box chars
		printw("┌─┐┌─┐┌─┐┌─┐┌─┐\n");
		printw("│ ││ ││ ││ ││ │\n");
		printw("└─┘└─┘└─┘└─┘└─┘\n");*/
	}
	move(2, 1);

	int guesses = 0;
	while (guesses < 6) {
		int i;
		int message = 0;
		int letter = 0;
		char guess[6];
		guess[5] = '\0';
		while (1) {
			int ch = getch();
			if (ch == KEY_ENTER || ch == '\n') {
				if (letter != 5) {
					move(2 + guesses * 3, 16);
					addstr("Not enough letters");
					message = 1;
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
					move(2 + guesses * 3, 16);
					addstr("Not in word list");
					message = 1;
					continue;
				}
				break;
			}
			// apparently the backspace return depends on platform???
			if ((ch == KEY_BACKSPACE || ch == 127 || ch == '\b') && letter > 0) {
				letter--;
				move(2 + guesses * 3, 1 + letter * 3);
				addch(' '); // after adding a character, cursor moves automatically
			}
			if (letter >= 5) continue;
			if (ch >= 97 && ch <= 122) ch -= 32;
			if (ch >= 65 && ch <= 90) {
				move(2 + guesses * 3, 1 + letter * 3);
				addch(ch);
				guess[letter] = ch;
				letter++;
				if (message) {
					move(2 + guesses * 3, 16);
					clrtoeol();
					message = 0;
				}
			}
		}
		if (strcmp(guess, msg.word) == 0) {
			move(2 + guesses * 3, 16);
			move(1 + guesses * 3, 0);
			chgat(15, A_BOLD | A_ALTCHARSET, 2, NULL);
			// If I don't have A_ALTCHARSET, the box chars become random letters
			// If I do have it, some of the normal letters become not letters
			// solution: replace guess
			move(2 + guesses * 3, 0);
			chgat(15, A_BOLD | A_ALTCHARSET, 2, NULL);

			for (i = 0; i < 5; i++) {
				move(2 + guesses * 3, 1 + i * 3);
				addch(guess[i] | A_BOLD | COLOR_PAIR(2));
			}

			move(3 + guesses * 3, 0);
			chgat(15, A_BOLD | A_ALTCHARSET, 2, NULL);
			break;
		}

		char outOfPlaceLetters[6];
		strncpy(outOfPlaceLetters, msg.word, 6);
		for (i = 0; i < 5; i++) {
			if (guess[i] == msg.word[i]) {
				move(1 + guesses * 3, i * 3);
				chgat(3, A_BOLD | A_ALTCHARSET, 2, NULL);
				move(2 + guesses * 3, i * 3);
				chgat(3, A_BOLD | A_ALTCHARSET, 2, NULL);
				// have to replace letter---see above
				move(2 + guesses * 3, 1 + i * 3);
				addch(guess[i] | A_BOLD | COLOR_PAIR(2));
				move(3 + guesses * 3, i * 3);
				chgat(3, A_BOLD | A_ALTCHARSET, 2, NULL);
				outOfPlaceLetters[i] = '\0';
			} else {
				int j;
				for (j = 0; j < 5; j++) {
					if (guess[i] == outOfPlaceLetters[j]) {
						move(1 + guesses * 3, i * 3);
						chgat(3, A_BOLD | A_ALTCHARSET, 1, NULL);
						move(2 + guesses * 3, i * 3);
						chgat(3, A_BOLD | A_ALTCHARSET, 1, NULL);
						// have to replace letter---see above
						move(2 + guesses * 3, 1 + i * 3);
						addch(guess[i] | A_BOLD | COLOR_PAIR(1));
						move(3 + guesses * 3, i * 3);
						chgat(3, A_BOLD | A_ALTCHARSET, 1, NULL);
						outOfPlaceLetters[j] = '\0';
					}
				}
			}
		}
		guesses++;
	}
	move(19, 0);
	printw("Guesses: %d\n", guesses + 1);
	if (guesses == 0) printw("The word was %s.\n", msg.word);
	printw("Press any key to continue\n");
	getch();
	clear();

	struct stat statStat;
	err = stat("./client_stats.bin", &statStat);
	struct stats stats;
	int statsFd = open("./client_stats.bin", O_RDWR | O_CREAT, 0644);
	if (err == 0) {
		read(statsFd, &stats, sizeof(stats));
		stats.played++;
		if (guesses == 6) {
			stats.streak = 0;
		} else {
			stats.streak++;
			stats.won++;
			if (stats.streak >= stats.maxStreak) stats.maxStreak = stats.streak;
			stats.guessDistribution[guesses]++;
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
		if (guesses == 6) {
			stats.streak = 0;
			stats.maxStreak = 0;
			stats.won = 0;
		} else {
			stats.streak = 1;
			stats.maxStreak = 1;
			stats.won = 1;
			stats.guessDistribution[guesses]++;
		}
	}
	lseek(statsFd, 0, SEEK_SET);
	write(statsFd, &stats, sizeof(stats));

	printw("Statistics:\n");
	// TODO make fancy guess distribution
	printw("Guess distribution: \n");
	printw("\t1 guess:\t%d\n", stats.guessDistribution[0]);
	printw("\t2 guesses:\t%d\n", stats.guessDistribution[1]);
	printw("\t3 guesses:\t%d\n", stats.guessDistribution[2]);
	printw("\t4 guesses:\t%d\n", stats.guessDistribution[3]);
	printw("\t5 guesses:\t%d\n", stats.guessDistribution[4]);
	printw("\t6 guesses:\t%d\n", stats.guessDistribution[5]);
	printw("Number of times played: %d\n", stats.played);
	printw("Percent won: %.2f%%\n", (float)stats.won / stats.played * 100);
	printw("Streak: %d\n", stats.streak);
	printw("Max streak: %d\n", stats.maxStreak);
	printw("Press any key to continue.\n");
	getch();

	// send to server

	msg.guesses = guesses; // zero-indexed
	msg.stats = stats;
	write(sd, &msg, sizeof(msg));

	// get total stats from server
	read(sd, &msg, sizeof(msg));
	printw("\nGame-wide statistics:\n");
	printw("Guess distribution: \n");
	printw("\t1 guess:\t%d\n", msg.stats.guessDistribution[0]);
	printw("\t2 guesses:\t%d\n", msg.stats.guessDistribution[1]);
	printw("\t3 guesses:\t%d\n", msg.stats.guessDistribution[2]);
	printw("\t4 guesses:\t%d\n", msg.stats.guessDistribution[3]);
	printw("\t5 guesses:\t%d\n", msg.stats.guessDistribution[4]);
	printw("\t6 guesses:\t%d\n", msg.stats.guessDistribution[5]);
	printw("Number of times played: %d\n", msg.stats.played);
	printw("Percent won: %.2f%%\n", (float)msg.stats.won / msg.stats.played * 100);
	printw("Max streak: %d\n", msg.stats.maxStreak);

	printw("Press any key to exit.\n");
	getch();
	endwin();
}
