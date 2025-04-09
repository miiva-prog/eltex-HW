CC = gcc
CFLAGS = -Wall -pthread
OBJ = main.o production.o
TARGET = production

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c production.h
	$(CC) $(CFLAGS) -c main.c

production.o: production.c production.h
	$(CC) $(CFLAGS) -c production.c

clean:
	rm -f $(OBJ) $(TARGET)