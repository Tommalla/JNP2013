.PHONY: clean final_test

#FIXME change -g to -O2 for final tests
CXXFLAGS=$(CFLAGS) -std=c++11 -Wall -pedantic -g
HEADER=../../virus_genealogy.h
VALGRIND_OPTS=--error-exitcode=15 --leak-check=full --track-origins=yes --suppressions=valgrind_dl_main

final: final_test
	valgrind $(VALGRIND_OPTS) ./final_test

final_test: virus_genealogy_test.cc $(HEADER)
	g++ $(CXXFLAGS) virus_genealogy_test.cc -DTEST_NUM=$(testid) -o final_test

all: main final
	./main

main: main.cpp $(HEADER)
	g++ $(CXXFLAGS) main.cpp -o main

test1: test1.cc $(HEADER)
	g++ $(CXXFLAGS) test1.cc -o test1

clean:
	rm -rf *.o
