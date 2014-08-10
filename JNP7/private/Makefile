.PHONY=clean

CXXFLAGS=-Wall -g -std=c++11

test: 42
	valgrind ./42

42: ../42.cc
	g++ $(CXXFLAGS) ../42.cc -o 42
