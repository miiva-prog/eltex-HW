CXX = gcc
CXXFLAGS = -Wall -Wextra -Werror

all: abonent

abonent: main.o abonent.o
	$(CXX) -o abonent main.o abonent.o

main.o: main.c abonent.h
	$(CXX) $(CXXFLAGS) -c main.c

abonent.o: abonent.c abonent.h
	$(CXX) $(CXXFLAGS) -c abonent.c

clean:
	rm -f *.o abonent