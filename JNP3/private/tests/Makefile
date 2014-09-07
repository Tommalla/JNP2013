CXXFLAGS=$(CFLAGS) -std=c++11 -O2 -Wall

all: test0
	./test0

test0: test0.cc sejf.o kontroler.o
	g++ $(CXXFLAGS) sejf.o kontroler.o test0.cc -o test0

sejf.o: ../../sejf.cc ../../sejf.h
	g++ $(CXXFLAGS) ../../sejf.cc -c -o sejf.o

kontroler.o: ../../kontroler.cc ../../kontroler.h
	g++ $(CXXFLAGS) ../../kontroler.cc -c -o kontroler.o

clean:
	rm -rf *.o test0
