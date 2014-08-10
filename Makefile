# Tomasz Wawreniuk i Tomasz Zakrzewski
# JNP 2013/2014, Zadanie 2
CXXFLAGS= -std=c++11
CFLAGS= -Wall --pedantic
DBGEXT=.dbg.o
RLSEXT=.rls.o

OEXT=$(RLSEXT)
NOEXT=$(DBGEXT)

ifeq ($(debuglevel),1)
	CFLAGS+=-DDEBUG -g
	OEXT=$(DBGEXT)
	NOEXT=$(RLSEXT)
else
	CFLAGS+=-O2
endif

CXX= g++ $(CFLAGS) $(CXXFLAGS)
CC= gcc $(CFLAGS)

all: network$(OEXT) growingnet$(OEXT)

clean:
	rm -rf *.o network_test1

network$(OEXT): network.cc network.h
	rm -rf network$(NOEXT)
	$(CXX) network.cc -c -o network$(OEXT)
	cp network$(OEXT) network.o

growingnet$(OEXT): network$(OEXT) growingnet.h growingnet.cc
	rm -rf growingnet$(NOEXT)
	$(CXX) growingnet.cc -c -o growingnet$(OEXT)
	cp growingnet$(OEXT) growingnet.o

.PHONY: all clean