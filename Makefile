.DEFAULT=all
.PHONY=all tests

all: network.o growingnet.o

network.o: network.cc network.h growingnet.o
	g++ -g -Wall -std=c++11 --pedantic network.cc -c -o network.o

growingnet.o: growingnet.h growingnet.cc
	g++ -g -Wall -std=c++11 --pedantic growingnet.cc -c -o growingnet.o

tests: network_test1
	./network_test1

network_test1: network.o growingnet.o network_test1.c
	gcc -g -Wall -O2 -c network_test1.c -o network_test1.o
	g++ -g network_test1.o growingnet.o network.o -o network_test1
