#include <stdio.h>
#include <stdlib.h>
#include "ll.h"
#include "lib.h"

int letter_index(char letter) {
	char artist_first_letter = letter;
	if (artist_first_letter >= 97 && artist_first_letter <= 122) artist_first_letter -= 32;
	if (artist_first_letter >= 65 && artist_first_letter <= 90) {
		return artist_first_letter - 65;
	}
	return 26;
}
struct song_node** make_library() {
	struct song_node** library = malloc(sizeof(struct song_node*) * 27);
	int i;
	for (i = 0; i < 27; i++) library[i] = NULL;
	return library;
}

//struct song_node** library_add_song(char* name, char* artist);
void library_add_song(struct song_node** library, char* name, char* artist) {
	int ind = letter_index(*artist);
	library[ind] = insert_order(library[ind], name, artist);
}
struct song_node* library_find_song(struct song_node** library, char* name, char* artist) {
	struct song_node* list = library[letter_index(*artist)];
	while (list != NULL) {
		if (alphabetic_strcmp(list->name, name) == 0 && alphabetic_strcmp(list->artist, artist) == 0) return list;
		list = list->next;
	}
	return list;
}
struct song_node* library_find_artist(struct song_node** library, char* artist) {
	struct song_node* list = library[letter_index(*artist)];
	while (list != NULL) {
		if (alphabetic_strcmp(list->artist, artist) == 0) return list;
		list = list->next;
	}
	return list;
}

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
	struct song_node* list = library[letter_index(*artist)];
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

void library_remove_song(struct song_node** library, char* name, char* artist) {
	library[letter_index(*artist)] = remove_song(library[letter_index(*artist)], name, artist);
}
void clear_library(struct song_node** library) {
	for (int i = 0; i < 27; i++) {
		if (library[i] != NULL) {
			library[i] = free_list(library[i]);
		}
	}
}
