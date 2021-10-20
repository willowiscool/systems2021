struct song_node {
	char* name;
	char* artist;
	struct song_node *next;
};

struct song_node* insert_front(struct song_node* list, char* name, char* artist);
struct song_node* insert_order(struct song_node* list, char* name, char* artist);
int song_comp(struct song_node* a, char* name, char* artist);
int alphabetic_strcmp(char* a, char* b);

void print_list(struct song_node* list);
struct song_node* find_song(struct song_node* list, char* name, char* artist);
struct song_node* first_song(struct song_node* list, char* artist);
struct song_node* random_song(struct song_node* list);

struct song_node* remove_song(struct song_node* list, char* name, char* artist);

struct song_node* free_list(struct song_node* list);
