#include "strings.h"
#include <stddef.h> // used for NULL

int mystrlen(char *s) {
	int len = 0;
	while (*s++) len++;
	return len;
}

char *mystrcpy(char *dest, char *source) {
	char *originalDest = dest;
	while (*source) {
		*dest = *source;
		dest++;
		source++;
	}
	// don't forget the null terminator!
	*dest = *source;
	return originalDest;
}

char *mystrncpy(char *dest, char *source, int n) {
	char *originalDest = dest;
	while (n--) {
		*dest = *source;
		dest++;
		source++;
	}
	return originalDest;
}

char *mystrcat(char *dest, char *source) {
	char *originalDest = dest;
	while (*dest++); //get to null terminator
	dest--; // overshot (will return 0 and then add again)
	mystrcpy(dest, source);
	return originalDest;
}

char *mystrncat(char *dest, char *source, int n) {
	char *originalDest = dest;
	while (*dest++); //get to null terminator
	dest--; // overshot (will return 0 and then add again)
	mystrncpy(dest, source, n);
	return originalDest;
}

int mystrcmp(char *s1, char *s2) {
	while (*s1 && *s2) {
		if (*s1 > *s2) return 1;
		if (*s1 < *s2) return -1;
		s1++;
		s2++;
	}
	if (*s1) return 1;
	if (*s2) return -1;
	return 0;
}

char *mystrchr(char *s, char c) {
	while (*s && *s++ != c);
	if (*s == '\0')
		if (*s == c) return s;
		else return NULL;
	return s - 1;
}

// finds s2 in s1
char *mystrstr(char *s1, char *s2) {
	while (*s1) {
		char *pos = s1;
		char *s2pos = s2;
		int equal = 1;
		while (*s2pos && *pos) {
			if (*s2pos != *pos) {
				equal = 0;
			}
			s2pos++;
			pos++;
		}
		if (equal) return s1;
		s1++;
	}
	return NULL;
}
