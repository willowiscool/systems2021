#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct numberName {
	char* str;
	int i;
	struct numberName *next;
};

void print_list(struct numberName* node) {
	while (node->next) {
		printf("%d is named %s\n", node->i, node->str);
		node = node->next;
	}
	//probably smarter way to do this but cba
	printf("%d is named %s\n", node->i, node->str);
}
struct numberName* insert_front(struct numberName* first, int i, char* str) {
	struct numberName* nn = malloc(sizeof(struct numberName));
	nn->str = malloc(strlen(str));
	strcpy(nn->str, str);
	nn->i = i;
	nn->next = first;
	return nn;
}
struct numberName* free_list(struct numberName* first) {
	struct numberName* node = first;
	while (node->next) {
		free(node->str);
		struct numberName* next = node->next;
		free(node);
		node = next;
	}
	free(node->str);
	free(node);
	return first;
}

int main() {
	// create initial nn
	struct numberName* nn = malloc(sizeof(struct numberName));
	nn->str = malloc(4);
	strcpy(nn->str, "Six");
	nn->i = 6;

	// test functions
	nn = insert_front(nn, 5, "Five");
	nn = insert_front(nn, 4, "Four");
	nn = insert_front(nn, 3, "Three");
	nn = insert_front(nn, 2, "Two");
	nn = insert_front(nn, 1, "One");

	print_list(nn);

	free_list(nn);

	if (nn == NULL) printf("Successfully freed\n");
}
