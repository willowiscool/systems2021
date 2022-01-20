CFLAGS = -lncurses

all: server.o client.o
	gcc -o server server.o ${CFLAGS} 
	gcc -o client client.o ${CFLAGS} 

server.o: server.c server.h types.h
	gcc -c server.c server.h types.h

client.o: client.c types.h
	gcc -c client.c types.h
