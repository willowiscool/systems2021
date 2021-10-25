all: ll.o lib.o combined_test.o
	gcc -o test combined_test.o ll.o lib.o

ll.o: ll.c ll.h
	gcc -c ll.c ll.h

lib.o: lib.c lib.h
	gcc -c lib.c lib.h

combined_test.o: combined_test.c
	gcc -c combined_test.c

run:
	./test
