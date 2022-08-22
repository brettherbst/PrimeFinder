findPrimes: main.o
	g++ main.o -o findPrimes

main.o: main.cpp
	g++ -Wall -pthread -std=c++20 -c main.cpp

clean:
	rm *.o findPrimes