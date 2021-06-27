despised: main.o
	g++ main.o -o despised
main.o: main.cpp problem.h
	g++ -c main.cpp -o main.o
