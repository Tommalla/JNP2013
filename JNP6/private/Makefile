.PHONY: clean test_final final

CFLAGS = -std=c++11 -Wall -pedantic -g

final: test_final
	./test_final > out

test_final:	mojagrubaryba.o zadanie6/grubaryba_tests.cc
	g++ $(CFLAGS) -DTEST_NUM=$(testnum) zadanie6/grubaryba_tests.cc mojagrubaryba.o -o test_final

test: example
	./example

example: mojagrubaryba.o grubaryba_example.cc
	g++ $(CFLAGS) grubaryba_example.cc mojagrubaryba.o -o example

mojagrubaryba.o: ../mojagrubaryba.h ../mojagrubaryba.cc
	g++ $(CFLAGS) ../mojagrubaryba.cc -c -o mojagrubaryba.o

clean:
	rm -rf *.o
