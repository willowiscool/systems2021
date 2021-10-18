#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

void print_list(struct numberName* node) {
	printf("[\n");
	while (node) {
		printf("\t%d is named %s\n", node->i, node->str);
		node = node->next;
	}
	printf("]\n");
}
struct numberName* insert_front(struct numberName* first, int i, char* str) {
	struct numberName* nn = malloc(sizeof(struct numberName));
	nn->str = malloc(strlen(str));
	strcpy(nn->str, str);
	nn->i = i;
	nn->next = first;
	return nn;
}
struct numberName* remove_node(struct numberName* first, int i) {
	struct numberName* node = first;
	struct numberName* previous = NULL;
	while (node) {
		if (node->i == i) {
			struct numberName* next = node->next;
			free(node->str);
			free(node);
			if (previous == NULL) {
				return next;
			}
			previous->next = next;
			return first;
		}
		previous = node;
		node = node->next;
	}
}
struct numberName* free_list(struct numberName* first) {
	struct numberName* node = first;
	while (node) {
		free(node->str);
		struct numberName* next = node->next;
		free(node);
		node = next;
	}
	return first;
}

int main() {
	printf("Print null list");
	print_list((struct numberName*) 0);
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

	printf("Removing odd numbers\n");
	nn = remove_node(nn, 1);
	nn = remove_node(nn, 3);
	nn = remove_node(nn, 5);

	print_list(nn);

	free_list(nn);

	if (nn == NULL) printf("Successfully freed\n");
}
