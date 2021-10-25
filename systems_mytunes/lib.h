struct song_node** make_library();

//struct song_node** library_add_song(char* name, char* artist);
void library_add_song(struct song_node** library, char* name, char* artist);
struct song_node* library_find_song(struct song_node** library, char* name, char* artist);
struct song_node* library_find_artist(struct song_node** library, char* artist);

void print_letter(struct song_node** library, char letter);
void print_artist(struct song_node** library, char* artist);
void print_library(struct song_node** library);
void shuffle(struct song_node** library);

//struct song_node** library_remove_song(char* name, char* artist);
void library_remove_song(struct song_node** library, char* name, char* artist);
struct song_node** clear_library(struct song_node** library);
