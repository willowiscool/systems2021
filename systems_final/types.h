#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <time.h>

struct stats {
	int guessDistribution[6];
	int played;
	int won; // win % calculated with these
	int streak;
	int maxStreak;
};

struct message {
	char word[6];
	int guesses; // zero-indexed
	struct stats stats;
	time_t timeGenerated;
};

#endif // MESSAGE_H_
