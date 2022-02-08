#include "utils.h"
#include "types.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct stats writeStats(char* filename, int guesses, int server) {
	struct stat statStat;
	int err = stat(filename, &statStat);
	struct stats stats;
	int statsFd = open(filename, O_RDWR | O_CREAT, 0644);
	if (err == 0) {
		read(statsFd, &stats, sizeof(stats));
		stats.played++;
		if (guesses != 6) {
			// server sets streak to -1
			if (!server) {
				stats.streak++;
				if (stats.streak >= stats.maxStreak) stats.maxStreak = stats.streak;
			}
			stats.won++;
			stats.guessDistribution[guesses]++;
		} else {
			if (stats.streak >= 0) stats.streak = 0;
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
		if (server) stats.streak = -1;
	}
	lseek(statsFd, 0, SEEK_SET);
	write(statsFd, &stats, sizeof(stats));
	return stats;
}
