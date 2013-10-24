.DEFAULT=all

all: network.o growingnet.o

network.o: network.cc network.h
	g++ -Wall --pedantic network.cc -c -o network.o

growingnet.o: network.o growingnet.h growingnet.cc
	g++ -Wall --pedantic network.o growingnet.cc -c -o growingnet.o