// this is just a file combining the test.c and ll_test.c files
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ll.h"
#include "lib.h"

int main() {
	printf("Testing compare function\n");

	struct song_node test;
	test.name = "BBB";
	test.artist = "AAA";
	printf("\tBBB by AAA vs CCC by CCC: %d\n", song_comp(&test, "CCC", "CCC"));
	printf("\tBBB by AAA vs AAA by AAA: %d\n", song_comp(&test, "AAA", "AAA"));

	struct song_node* list = NULL;
	printf("Inserting Sophie's music\n");
	list = insert_front(list, "whole NEW WORLD", "SOPHIE");
	list = insert_front(list, "FACESHOPPING", "Sophie");
	printf("Printing list\n");
	print_list(list);

	printf("Inserting in order: PONYBOY\n");
	list = insert_order(list, "ponyboy", "sophie");
	print_list(list);

	printf("Finding ponyboy:\n");
	struct song_node* ponyboy = find_song(list, "ponyboy", "sophie");
	printf("\tFound: %s by %s\n", ponyboy->name, ponyboy->artist);
	printf("Finding not okay:\n");
	struct song_node* not_okay = find_song(list, "Not Okay", "sophie");
	printf("\tFound: %p\n", not_okay);

	printf("First sophie song:\n");
	struct song_node* first_sophie = first_song(list, "sOpHiE");
	printf("\tFound: %s by %s\n", first_sophie->name, first_sophie->artist);

	printf("Inserting Mr. DW's boring music (code done by vim macro)\n");
	list = insert_order(list, "thunderstruck", "ac/dc");
	list = insert_order(list, "alive", "pearl jam");
	list = insert_order(list, "even flow", "pearl jam");
	list = insert_order(list, "yellow ledbetter", "pearl jam");
	list = insert_order(list, "time", "pink floyd");
	list = insert_order(list, "paranoid android", "radiohead");
	list = insert_order(list, "street spirit (fade out)", "radiohead");
	print_list(list);

	srand(time(NULL));
	printf("Random song:\n");
	struct song_node* rand_song = random_song(list);
	printf("\tFound: %s by %s\n", rand_song->name, rand_song->artist);

	printf("Removing ac/dc and pink floyd:\n");
	list = remove_song(list, "thunderstruck", "ac/dc");
	list = remove_song(list, "time", "pink floyd");
	print_list(list);

	printf("Freeing everything:\n");
	list = free_list(list);
	printf("\tList now: %p\n", list);

	printf("\n=====\n=====\n=====\n\n");

	struct song_node **library = make_library();
	library_add_song(library, "10-20-40", "Rina Sawayama");
	library_add_song(library, "thunderstruck", "ac/dc");
	library_add_song(library, "alive", "pearl jam");
	library_add_song(library, "even flow", "pearl jam");
	library_add_song(library, "yellow ledbetter", "pearl jam");
	library_add_song(library, "time", "pink floyd");
	library_add_song(library, "paranoid android", "radiohead");
	library_add_song(library, "street spirit (fade out)", "radiohead");
	print_library(library);
	printf("Printing letter p and then z:\n");
	print_letter(library, 'p');
	print_letter(library, 'z');
	printf("Printing radiohead:\n");
	print_artist(library, "Radiohead");
	printf("Finding paranoid android:\n");
	struct song_node* paranoid_android = library_find_song(library, "paranoid android", "radiohead");
	printf("\tfound %s by %s\n", paranoid_android->name, paranoid_android->artist);
	printf("Finding radiohead:\n");
	struct song_node* radiohead = library_find_artist(library, "radiohead");
	print_list(radiohead);
	printf("Removing paranoid android:\n");
	library_remove_song(library, "paranoid Android", "RadioHead");
	print_library(library);
	printf("Shuffling:\n");
	shuffle(library);
	printf("Clearing library:\n");
	clear_library(library);
	print_library(library);
}
