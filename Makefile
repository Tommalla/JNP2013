CXXFLAGS= -std=c++11
# Tomasz Wawreniuk i Tomasz Zakrzewski
# JNP 2013/2014, Zadanie 2

CFLAGS= -Wall --pedantic
OEXT=.o

ifeq ($(debuglevel),1)
	CFLAGS+=-DDEBUG -g
	OEXT=.dbg.o
else
	CFLAGS+=-O2
endif

CXX= g++ $(CFLAGS) $(CXXFLAGS)
CC= gcc $(CFLAGS)

all: network$(OEXT) growingnet$(OEXT)

clean:
	rm -rf *.o network_test1

network$(OEXT): network.cc network.h
	$(CXX) network.cc -c -o network$(OEXT)

growingnet$(OEXT): network$(OEXT) growingnet.h growingnet.cc
	$(CXX) growingnet.cc -c -o growingnet$(OEXT)

tests: network_test1
	./network_test1

network_test1: network$(OEXT) growingnet$(OEXT) network_test1.c
	$(CC) -c network_test1.c -o network_test1.o
	$(CXX) network_test1.o growingnet$(OEXT) network$(OEXT) -o network_test1

network_test2: network$(OEXT) growingnet$(OEXT) network_test2.c
	$(CC) -c network_test2.c -o network_test2.o
	$(CXX) network_test2.o growingnet$(OEXT) network$(OEXT) -o network_test2

network_test3: network$(OEXT) growingnet$(OEXT) network_test3.c
	$(CC) -c network_test3.c -o network_test3.o
	$(CXX) network_test3.o growingnet$(OEXT) network$(OEXT) -o network_test3
	./network_test3

.PHONY: all tests