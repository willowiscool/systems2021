#ifndef MESSAGE_H_
#define MESSAGE_H_

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
};

#endif // MESSAGE_H_
