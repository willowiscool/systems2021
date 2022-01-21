CFLAGS = -lncurses

all: server.o client.o utils.o
	gcc -o server server.o utils.o ${CFLAGS}
	gcc -o client client.o utils.o ${CFLAGS}

server.o: server.c server.h types.h utils.h
	gcc -c server.c server.h types.h

client.o: client.c types.h utils.h
	gcc -c client.c types.h

utils.o: utils.c utils.h types.h
	gcc -c utils.c utils.h types.h
