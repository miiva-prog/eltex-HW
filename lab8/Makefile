LIB = libcalc.so

TARGET = main

SRCS = add.c sub.c mul.c div.c

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -O2 -fPIC
LDFLAGS = -shared

all: $(TARGET)

$(LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET): main.c $(LIB)
	$(CC) $(CFLAGS) main.c -o $@ -L. -lcalc

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB) $(TARGET)

.PHONY: all clean