#include <stdio.h>
#include "ll.h"
#include "lib.h"

int main() {
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
	printf("Clearing library:\n");
	clear_library(library);
	print_library(library);
}
