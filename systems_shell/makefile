all: main.o run.o
	gcc -o main main.o run.o

main.o: main.c
	gcc -c main.c

run.o: run.c
	gcc -c run.c

run:
	./main
