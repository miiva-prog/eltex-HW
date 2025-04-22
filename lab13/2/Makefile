CC = gcc
CFLAGS = -Wall -Wextra -pthread -lrt -lncurses

OBJS = chat.o

all: server client

server: server.o $(OBJS)
	$(CC) -o server server.o $(OBJS) $(CFLAGS)

client: client.o $(OBJS)
	$(CC) -o client client.o $(OBJS) $(CFLAGS)

clean:
	rm -f *.o server client