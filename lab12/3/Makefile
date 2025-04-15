CC = gcc
CFLAGS = -Wall -Wextra
OBJ = main.o bash.o

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

main.o: main.c bash.h
	$(CC) $(CFLAGS) -c main.c

bash.o: bash.c bash.h
	$(CC) $(CFLAGS) -c bash.c

clean:
	rm -f *.o main