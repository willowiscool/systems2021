all: main.o
	gcc -o main main.o

main.o: main.c
	gcc -c main.c

run:
	rm random_numbers.bin
	./main
