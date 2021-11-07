#include <stdio.h>
#include <stdlib.h>
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
		} else if (strcmp(argv[1], "-read_data") == 0) {
			int count = 0;
			struct pop_entry* data = readData(&count);
			if (data == NULL) return -1;
			displayData(count, data);
			return 0;
		} else if (strcmp(argv[1], "-add_data") == 0) {
			return addData();
		} else if (strcmp(argv[1], "-update_data") == 0) {
			return updateData();
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

	return writeData(data, sizeof(data));
}
int writeData(struct pop_entry* data, int size) {
	int fd = open("./nyc_pop.data", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
		printf("Error opening data file: %s\n", strerror(errno));
		return -1;
	}
	int b = write(fd, data, size);
	if (b != size) {
		printf("Error writing data file: %s\n", strerror(errno));
		return -1;
	}
	printf("wrote %d bytes to nyc_pop.data\n", b);
	return 0;
}
struct pop_entry *readData(int *count) {
	int fd = open("./nyc_pop.data", O_RDONLY);
	if (fd == -1) {
		printf("Error opening data file: %s\n", strerror(errno));
		return NULL;
	}
	struct stat status;
	int err = fstat(fd, &status);
	if (err == -1) {
		printf("Error statting data file: %s\n", strerror(errno));
		return NULL;
	}
	*count = status.st_size / sizeof(struct pop_entry);
	struct pop_entry *data = malloc(status.st_size);
	err = read(fd, data, status.st_size);
	if (err == -1) {
		printf("Error reading data file: %s\n", strerror(errno));
		return NULL;
	}
	return data;
}
void displayData(int count, struct pop_entry data[]) {
	int i;
	for (i = 0; i < count; i++) {
		printf("%d: year: %d\tboro: %s\tpop: %d\n", i, data[i].year, data[i].boro, data[i].population);
	}
}

struct pop_entry getInput() {
	printf("Enter year boro pop: ");
	char buf[1000];
	fgets(buf, sizeof(buf), stdin);
	struct pop_entry result;
	sscanf(buf, "%d %s %d\n", &result.year, result.boro, &result.population);
	return result;
}
int addData() {
	struct pop_entry new_data = getInput();
	int fd = open("./nyc_pop.data", O_WRONLY | O_APPEND);
	if (fd == -1) {
		printf("Error opening data file: %s\n", strerror(errno));
		return -1;
	}
	int b = write(fd, &new_data, sizeof(new_data));
	if (b != sizeof(new_data)) {
		printf("Error appending data file: %s\n", strerror(errno));
		return -1;
	}
	printf("Appended data to file: year: %d\tboro: %s\tpop: %d\n", new_data.year, new_data.boro, new_data.population);
	return 0;
}
int updateData() {
	int count = 0;
	struct pop_entry* data = readData(&count);
	if (data == NULL) return -1;
	displayData(count, data);
	char buf[1000];
	printf("entry to update: ");
	fgets(buf, sizeof(buf), stdin);
	int ind = 0;
	sscanf(buf, "%d\n", &ind);
	struct pop_entry input = getInput();
	data[ind] = input;
	return writeData(data, count * sizeof(struct pop_entry));
}
