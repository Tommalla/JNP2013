.PHONY: clean

CXXFLAGS = $(CFLAGS) -std=c++11 -Wall -pedantic -g

tests: main miel test_holding

main: main.cpp ../holding.hh
	g++ $(CXXFLAGS) main.cpp -o main

miel: miel.cpp ../holding.hh
	g++ $(CXXFLAGS) miel.cpp -o miel

test_holding: test_holding.cc ../holding.hh
	g++ $(CXXFLAGS) test_holding.cc -o test_holding

clean:
	rm -rf main miel test_holding
