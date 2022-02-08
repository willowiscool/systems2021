#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ll.h"

struct song_node* insert_front(struct song_node* list, char* name, char* artist) {
	struct song_node* new = malloc(sizeof(struct song_node));
	// strlen + 1 '\0' byte
	new->name = malloc(strlen(name) + 1);
	new->artist = malloc(strlen(artist) + 1);
	strcpy(new->name, name);
	strcpy(new->artist, artist);
	strcpy(new->name, name);
	new->next = list;
	return new;
}
struct song_node* insert_order(struct song_node* list, char* name, char* artist) {
	if (list == NULL) return insert_front(list, name, artist);
	if (song_comp(list, name, artist) > 0) return insert_front(list, name, artist);
	struct song_node* front = list;
	struct song_node* prev_list;
	while(list != NULL && song_comp(list, name, artist) < 0) {
		prev_list = list;
		list = list->next;
	}
	struct song_node* new = insert_front(list, name, artist);
	prev_list->next = new;
	return front;
}
// returns 1 if a > b, -1 if a < b, 0 otherwise
// TODO MAKE CASE INSENSITIVE
int song_comp(struct song_node* a, char* b_name, char* b_artist) {
	int artist_cmp = alphabetic_strcmp(a->artist, b_artist);
	if (artist_cmp != 0) return artist_cmp;
	return alphabetic_strcmp(a->name, b_name);
}

int alphabetic_strcmp(char* a, char* b) {
	while (*a || *b) {
		char acap = *a;
		char bcap = *b;
		if (acap >= 97 && acap <= 122) acap -= 32;
		if (bcap >= 97 && bcap <= 122) bcap -= 32;
		if (acap > bcap) return 1;
		if (acap < bcap) return -1;
		a++;
		b++;
	}
	if (*a) return 1;
	if (*b) return -1;
	return 0;
}

void print_list(struct song_node* list) {
	printf("[\n");
	while (list != NULL) {
		printf("\t%s by %s\n", list->name, list->artist);
		list = list->next;
	}
	printf("]\n");
}

// alphabetic_strcmp works here too
struct song_node* find_song(struct song_node* list, char* name, char* artist) {
	while (list != NULL) {
		if (alphabetic_strcmp(list->name, name) == 0 && alphabetic_strcmp(list->artist, artist) == 0) return list;
		list = list->next;
	}
	return list; // null
}
struct song_node* first_song(struct song_node* list, char* artist) {
	while (list != NULL) {
		if (alphabetic_strcmp(list->artist, artist) == 0) return list;
		list = list->next;
	}
	return list;
}

/* Cheap but not very equal
struct song_node* random_song(struct song_node* list) {
	if (list == NULL) return list;
	srand(time(NULL));
	int randint = rand();
	while (list->next != NULL && randint > (RAND_MAX / 2)) {
		list = list->next;
	}
	return list;
}
*/
struct song_node* random_song(struct song_node* list) {
	struct song_node* front = list;
	int length = 0;
	while (list != NULL) {
		length++;
		list = list->next;
	}
	int index = rand() % length;
	while (index-- > 0) front = front->next;
	return front;
}

struct song_node* remove_song(struct song_node* list, char* name, char* artist) {
	if (alphabetic_strcmp(list->artist, artist) == 0 && alphabetic_strcmp(list->name, name) == 0) {
		struct song_node* next = list->next;
		free_node(list);
		return next;
	}
	struct song_node* first = list;
	while (list->next != NULL) {
		struct song_node* prev = list;
		list = list->next;
		if (alphabetic_strcmp(list->artist, artist) == 0 && alphabetic_strcmp(list->name, name) == 0) {
			prev->next = list->next;
			free_node(list);
			return first;
		}
	}
}

struct song_node* free_list(struct song_node* list) {
	if (list == NULL) return NULL;
	while (list->next != NULL) {
		struct song_node* next = list->next;
		free_node(list);
		list = next;
	}
	free_node(list);
	return NULL;
}
struct song_node* free_node(struct song_node* node) {
	free(node->name);
	free(node->artist);
	free(node);
	return NULL;
}
