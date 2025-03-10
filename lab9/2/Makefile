CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lncurses

OBJ = main.o mc.o

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ) $(LDFLAGS)

main.o: main.c mc.h
	$(CC) $(CFLAGS) -c main.c

mc.o: mc.c mc.h
	$(CC) $(CFLAGS) -c mc.c

clean:
	rm -f *.o main *.txt