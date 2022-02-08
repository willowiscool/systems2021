#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	char *dirname;
	if (argc >= 2) dirname = argv[1];
	else {
		char buff[1000];
		printf("Enter a directory to list: ");
		fgets(buff, sizeof(buff), stdin);
		dirname = buff;
		if (dirname[strlen(dirname) - 1] == '\n') dirname[strlen(dirname) - 1] = '\0'; // erase newline
	}
	
	printf("dirname: '%s'\n", dirname);
	DIR *wd = opendir(dirname);
	if (wd == NULL) {
		printf("Error opening directory: %s\n", strerror(errno));
	}
	struct dirent *entry;
	entry = readdir(wd);
	if (entry == NULL) {
		printf("Empty directory\n");
		return 0;
	}
	int size = 0;
	while (entry != NULL) {
		struct stat stats;
		stat(entry->d_name, &stats);
		size += stats.st_size;
		printf("%s %s\n", entry->d_type == DT_DIR ? "d" : "f", entry->d_name);
		entry = readdir(wd);
	}
	printf("\ntotal size: %d bytes\n", size);
}
