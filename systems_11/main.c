#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
	// list all the files in the current directory
	// specify which files are directories
	// show the total size of the regular files in the directory (no subdirs)
	
	DIR *wd = opendir("./");
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
