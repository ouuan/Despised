despised: main.o
    $(CXX) main.o -o despised
main.o: main.cpp problem.h
    $(CXX) -c main.cpp -o main.o

.PHONY: clean
clean:
    rm -f main.o despised
