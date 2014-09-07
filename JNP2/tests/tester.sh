#!/bin/bash

(cd .. && make)

for i in tes*.cc
do
    echo "Wykonuje test - $i";
    gcc -std=c++0x -c $i -o tmp.o
    g++ tmp.o ../network.o ../growingnet.o -o spr
    ./spr
    g++ tmp.o ../growingnet.o ../network.o -o spr
    ./spr
    rm tmp.o spr
done

(cd .. && make clean)
