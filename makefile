all: clean compile link run

compile: 
	g++ -g -Wall -std=c++11 -Iinclude -c -o main.o main.cpp

link:
	g++ -o main main.o

clean:
	rm -f main.o main

run: clean
	./main
