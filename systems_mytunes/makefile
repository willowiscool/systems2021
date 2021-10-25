all: ll.o lib.o test.o
	gcc -o test test.o ll.o lib.o

ll.o: ll.c ll.h
	gcc -c ll.c ll.h

lib.o: lib.c lib.h
	gcc -c lib.c lib.h

test.o: test.c
	gcc -c test.c

run:
	./test
