.PHONY: clean

CFLAGS = -std=c++11 -Wall -pedantic -g

test: example
	./example

example: mojagrubaryba.o grubaryba_example.cc
	g++ $(CFLAGS) grubaryba_example.cc mojagrubaryba.o -o example

mojagrubaryba.o: ../mojagrubaryba.h ../mojagrubaryba.cc
	g++ $(CFLAGS) ../mojagrubaryba.cc -c -o mojagrubaryba.o
	
clean:
	rm -rf *.o
