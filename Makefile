.DEFAULT=all
.PHONY=all tests
CFLAGS=

ifeq (debuglevel, 1)
	CFLAGS=-ddebuglevel
else 
	CFLAGS=-dndebuglevel
endif

CXX= g++ $(CFLAGS)
CC= gcc $(CFLAGS)


all: network.o growingnet.o

network.o: network.cc network.h
	$(CXX) -Wall -std=c++11 --pedantic network.cc -c -o network.o

growingnet.o: network.o growingnet.h growingnet.cc
	$(CXX) -Wall -std=c++11 --pedantic network.o growingnet.cc -c -o growingnet.o

tests: network_test1
	./network_test1

network_test1: network.o growingnet.o network_test1.c
	$(CC) -Wall -O2 -c network_test1.c -o network_test1.o
	$(CXX) network_test1.o growingnet.o network.o -o network_test1
