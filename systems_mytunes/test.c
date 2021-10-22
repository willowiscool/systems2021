#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ll.h"

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
}
