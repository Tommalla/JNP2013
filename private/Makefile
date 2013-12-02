.PHONY: clean

tests: main miel


main: main.cpp ../holding.hh
	g++ -std=c++11 -Wall --pedantic -O2 main.cpp -o main

miel: miel.cpp ../holding.hh
	g++ -std=c++11 -Wall --pedantic -O2 miel.cpp -o miel

clean:
	rm -rf main
