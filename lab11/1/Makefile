CXX = gcc
CXXFLAGS = -Wall -Wextra -Werror

all: pth

pth: main.o pth.o
	$(CXX) -o pth main.o pth.o

main.o: main.c pth.h
	$(CXX) $(CXXFLAGS) -c main.c

pth.o: pth.c pth.h
	$(CXX) $(CXXFLAGS) -c pth.c

clean:
	rm -f *.o pth