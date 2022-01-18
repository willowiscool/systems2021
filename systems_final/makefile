all: server.o client.o
	gcc -o server server.o
	gcc -o client client.o

server.o: server.c server.h message.h
	gcc -c server.c server.h message.h

client.o: client.c message.h
	gcc -c client.c message.h
