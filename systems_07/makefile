all: main.o strings.o
	gcc -o main strings.o main.o

strings.o: strings.c strings.h
	gcc -c strings.c

main.o: main.c strings.h
	gcc -c main.c

run:
	./main
