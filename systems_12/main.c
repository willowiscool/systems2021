#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "main.h"

int main(int argc, char *argv[]) {
	if (argc == 2) {
		if (strcmp(argv[1], "-read_csv") == 0) {
			return readCSV();
		}
	}
	printf("Usage: %s OPTION\n\t-read_csv\tCreate a new data file out of the CSV\n\t-read_data\tDisplay the contents of the data file\n\t-add_data\tPut new data into the data file\n\t-update_data\tUpdate a specific entry in the data\n", argv[0]);
	return -1;
}

int readCSV() {
	int fd = open("./nyc_pop.csv", O_RDONLY);
	if (fd == -1) {
		printf("Error opening CSV file: %s\n", strerror(errno));
		return -1;
	}
	struct stat status;
	int err = fstat(fd, &status);
	if (err == -1) {
		printf("Error statting CSV file: %s\n", strerror(errno));
		return -1;
	}
	char file[status.st_size + 1];
	printf("reading nyc_pop.csv\n");
	err = read(fd, file, sizeof(file) - 1);
	if (err == -1) {
		printf("Error reading CSV file: %s\n", strerror(errno));
		return -1;
	}
	file[sizeof(file) - 1] = '\0';
	
	// find number of entries
	int i;
	int number_of_lines = 0;
	for (i = 0; i < sizeof(file); i++) {
		if (file[i] == '\n') number_of_lines++;
	}

	char *linePtr = file;
	while (*linePtr != '\n') linePtr++; // skip past first line
	struct pop_entry data[(number_of_lines - 1) * 5];
	for (i = 0; i < (number_of_lines - 1) * 5; i += 5) {
		int year, manhattan, brooklyn, queens, bronx, staten_island;
		int matched = sscanf(linePtr, "\n%d,%d,%d,%d,%d,%d\n", &year, &manhattan, &brooklyn, &queens, &bronx, &staten_island);
		if (matched != 6) {
			printf("Failed to match line!\n\tlinePtr: %s\n\tNumber matched: %d\n", linePtr, matched);
			return -1;
		}
		linePtr++;
		while (*linePtr != '\n') linePtr++; // next line

		data[i].year = year;
		data[i].population = manhattan;
		strcpy(data[i].boro, "Manhattan");
		data[i+1].year = year;
		data[i+1].population = brooklyn;
		strcpy(data[i+1].boro, "Brooklyn");
		data[i+2].year = year;
		data[i+2].population = queens;
		strcpy(data[i+2].boro, "Queens");
		data[i+3].year = year;
		data[i+3].population = bronx;
		strcpy(data[i+3].boro, "Bronx");
		data[i+4].year = year;
		data[i+4].population = staten_island;
		strcpy(data[i+4].boro, "Staten Island");
	}

	fd = open("./nyc_pop.data", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
		printf("Error opening data file: %s\n", strerror(errno));
		return -1;
	}
	int b = write(fd, data, sizeof(data));
	if (b != sizeof(data)) {
		printf("Error writing data file: %s\n", strerror(errno));
		return -1;
	}
	printf("wrote %d bytes to nyc_pop.data\n", b);
	return 0;
}
//struct pop_entry[] readData();
void displayData(int count, struct pop_entry data[]) {
	int i;
	for (i = 0; i < count; i++) {
		printf("%d: year: %d\tboro: %s\tpop: %d\n", i, data[i].year, data[i].boro, data[i].population);
	}
}

struct pop_entry getInput();
void addData();
void updateData();
