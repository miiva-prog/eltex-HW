CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = CLI
SOURCES = main.c CLI.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = CLI.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean