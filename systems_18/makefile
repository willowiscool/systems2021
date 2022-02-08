all: child.o parent.o
	gcc -o child child.o
	gcc -o parent parent.o

child.o: child.c
	gcc -c child.c

parent.o: parent.c
	gcc -c parent.c
