#!/bin/bash

for num in 100 200 201 202 300 301 302 303 304 305 306 307 308 309 310 311 400 401 402 403 500 501 502 503 600
do
	echo "Test: $num"
	make testnum=$num
	#g++ -g -Wall -std=c++11 -DTEST_NUM=$num mojagrubaryba.o zadanie6/grubaryba_tests.cc -o final_test
	#./final_test > out
	diff -bq out zadanie6/grubaryba_tests_$num.out
	if [ $? != 0 ]
	then
		echo "Error!"
		#break
	fi
done
