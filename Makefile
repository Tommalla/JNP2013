.DEFAULT=all
.PHONY=all tests

all: network.o growingnet.o

network.o: network.cc network.h
	g++ -Wall --pedantic network.cc -c -o network.o

growingnet.o: network.o growingnet.h growingnet.cc
	g++ -Wall --pedantic network.o growingnet.cc -c -o growingnet.o

tests: network_test1

network_test1: network.o growingnet.o network_test1.c
	gcc -Wall -O2 -c network_test1.c -o network_test1.o
	g++ network_test1.o growingnet.o network.o -o network_test1
	./network_test1