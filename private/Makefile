.PHONY: clean

#FIXME change -g to -O2 for final tests
CXXFLAGS=$(CFLAGS) -std=c++11 -Wall -pedantic -g
HEADER=../virus_genealogy.h

all: main
	./main

main: main.cpp $(HEADER)
	g++ $(CXXFLAGS) main.cpp -o main

clean:
	rm -rf *.o