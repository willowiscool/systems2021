#include <stdio.h>
#include <stdlib.h>
#include "ll.h"

struct song_node** make_library() {
	struct song_node** library = malloc(sizeof(struct song_node*) * 27);
	int i;
	for (i = 0; i < 27; i++) library[i] = NULL;
	return library;
}

//struct song_node** library_add_song(char* name, char* artist);
void library_add_song(struct song_node** library, char* name, char* artist) {
	char artist_first_letter = *artist;
	if (artist_first_letter >= 97 && artist_first_letter <= 122) artist_first_letter -= 32;
	if (artist_first_letter >= 65 && artist_first_letter <= 90) {
		library[artist_first_letter - 65] = insert_order(library[artist_first_letter - 65], name, artist);
		return;
	}
	library[26] = insert_order(library[26], name, artist);
}
struct song_node* library_find_song(struct song_node** library, char* name, char* artist);
struct song_node* library_find_artist(struct song_node** library, char* artist);

void print_letter(struct song_node** library, char letter) {
	if (letter >= 97 && letter <= 122) letter -= 32;
	if (letter >= 65 && letter <= 90) {
		print_list(library[letter - 65]);
		return;
	}
	// default to special characters ig
	print_list(library[26]);
	return;
}
void print_artist(struct song_node** library, char* artist) {
	struct song_node* list = library[26];
	char artist_first_letter = *artist;
	if (artist_first_letter >= 97 && artist_first_letter <= 122) artist_first_letter -= 32;
	if (artist_first_letter >= 65 && artist_first_letter <= 90) {
		list = library[artist_first_letter - 65];
	}
	printf("[\n");
	while(list != NULL) {
		if (alphabetic_strcmp(list->artist, artist) == 0) printf("\t%s by %s\n", list->name, list->artist);
		list = list->next;
	}
	printf("]\n");
}
void print_library(struct song_node** library) {
	int i;
	for (i = 0; i < 26; i++) {
		if (library[i] != NULL) {
			printf("===%c===\n", i + 65);
			print_list(library[i]);
		}
	}
	if (library[26] != NULL) {
		printf("=======\n");
		print_list(library[26]);
	}
}
void shuffle(struct song_node** library);
