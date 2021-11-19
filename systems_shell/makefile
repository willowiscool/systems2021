all: main.o run.o token.o
	gcc -o main main.o run.o token.o

main.o: main.c main.h run.h token.h
	gcc -c main.c main.h run.h token.h

run.o: run.c
	gcc -c run.c token.h

token.o: token.c
	gcc -c token.c token.h

run:
	./main
