#include <stdio.h>
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
}
