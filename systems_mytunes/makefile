all: ll.o test.o
	gcc -o test test.o ll.o

ll.o: ll.c ll.h
	gcc -c ll.c ll.h

test.o: test.c
	gcc -c test.c

run:
	./test
