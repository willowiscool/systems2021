struct numberName {
	char* str;
	int i;
	struct numberName *next;
};
void print_list(struct numberName* node);
struct numberName* insert_front(struct numberName* first, int i, char* str);
struct numberName* free_list(struct numberName* first);
struct numberName* remove_node(struct numberName* first, int i);
