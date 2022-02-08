#include <stdio.h>

int main() {
	char c = 'A';
	int i = 5;
	long l = 77;
	printf("Addresses of each variable:\n");
	printf("Hex\t\tChar: %p\tInt: %p\tLong: %p\n", &c, &i, &l);
	printf("Decimal\t\tChar: %lu\tInt: %lu\tLong: %lu\n\n", &c, &i, &l);
	// what do I notice? The int is 1 byte after the char (which is 1 byte long),
	// and the long is 4 bytes after the int (which is 4 bytes long)

	char *pc = &c;
	int *pi = &i;
	long *pl = &l;
	printf("Values of each pointer:\n");
	printf("Hex\t\tChar: %p\tInt: %p\tLong: %p\n", pc, pi, pl);
	printf("Decimal\t\tChar: %lu\tInt: %lu\tLong: %lu\n\n", pc, pi, pl);

	*pc = 'B';
	*pi = 6;
	*pl = 78;
	printf("New values, after being modified with pointers:\n");
	printf("Char: %c\tInt: %d\tLong: %ld\n\n", c, i, l);

	unsigned int ui = 238;
	// ipui = int pointer to unsigned int, similar for cpui
	int *ipui = &ui;
	char *cpui = &ui;
	printf("Unsigned int and its pointer:\n");
	printf("Int pointer: %p\tpoints to: %u\n", ipui, *ipui);
	printf("Char pointer: %p\tpoints to: %u\n", cpui, *cpui);
	printf("Decimal: %u\tHex: %x\n", ui, ui);
	printf("Individual bytes:\t%hhu %hhu %hhu %hhu\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	printf("Individual bytes (hex):\t%hhx %hhx %hhx %hhx\n\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));

	// could do in for loop cba
	printf("Adding one to each byte:\n");
	(*cpui)++;
	printf("Decimal: %u\tHex: %x\n", ui, ui);
	printf("Individual bytes:\t%hhu %hhu %hhu %hhu\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	printf("Individual bytes (hex):\t%hhx %hhx %hhx %hhx\n\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	(*(cpui + 1))++;
	printf("Decimal: %u\tHex: %x\n", ui, ui);
	printf("Individual bytes:\t%hhu %hhu %hhu %hhu\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	printf("Individual bytes (hex):\t%hhx %hhx %hhx %hhx\n\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	(*(cpui + 2))++;
	printf("Decimal: %u\tHex: %x\n", ui, ui);
	printf("Individual bytes:\t%hhu %hhu %hhu %hhu\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	printf("Individual bytes (hex):\t%hhx %hhx %hhx %hhx\n\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	(*(cpui + 3))++;
	printf("Decimal: %u\tHex: %x\n", ui, ui);
	printf("Individual bytes:\t%hhu %hhu %hhu %hhu\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	printf("Individual bytes (hex):\t%hhx %hhx %hhx %hhx\n\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));

	// copy paste is love, copy paste is life
	printf("Adding sixteen to each byte:\n");
	(*cpui) += 16;
	printf("Decimal: %u\tHex: %x\n", ui, ui);
	printf("Individual bytes:\t%hhu %hhu %hhu %hhu\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	printf("Individual bytes (hex):\t%hhx %hhx %hhx %hhx\n\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	(*(cpui + 1)) += 16;
	printf("Decimal: %u\tHex: %x\n", ui, ui);
	printf("Individual bytes:\t%hhu %hhu %hhu %hhu\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	printf("Individual bytes (hex):\t%hhx %hhx %hhx %hhx\n\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	(*(cpui + 2)) += 16;
	printf("Decimal: %u\tHex: %x\n", ui, ui);
	printf("Individual bytes:\t%hhu %hhu %hhu %hhu\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	printf("Individual bytes (hex):\t%hhx %hhx %hhx %hhx\n\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	(*(cpui + 3)) += 16;
	printf("Decimal: %u\tHex: %x\n", ui, ui);
	printf("Individual bytes:\t%hhu %hhu %hhu %hhu\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	printf("Individual bytes (hex):\t%hhx %hhx %hhx %hhx\n\n", *cpui, *(cpui + 1), *(cpui + 2), *(cpui + 3));
	return 0;
}
