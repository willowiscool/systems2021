all: server.o client.o
	gcc -o server server.o
	gcc -o client client.o

server.o: server.c server.h types.h
	gcc -c server.c server.h types.h

client.o: client.c types.h
	gcc -c client.c types.h
