LIBRARY_NAME = libcalc.a

TARGET = main

SRCS = add.c sub.c mul.c div.c

OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -O2

all: $(TARGET)

$(LIBRARY_NAME): $(OBJS)
	ar rcs $@ $^

$(TARGET): main.c $(LIBRARY_NAME)
	$(CC) $(CFLAGS) main.c -o $@ -L. -lcalc

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIBRARY_NAME) $(TARGET)

.PHONY: all clean