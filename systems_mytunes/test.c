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
}
