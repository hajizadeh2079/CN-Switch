CC = g++ -std=c++11

all: console.out system.out switch.out

console.out: console.o
	${CC} console.o -o console.out

system.out: system.o
	${CC} system.o -o system.out

switch.out: switch.o
	${CC} switch.o -o switch.out

console.o: console.cpp
	${CC} -c console.cpp

system.o: system.cpp
	${CC} -c system.cpp

switch.o: switch.cpp
	${CC} -c switch.cpp

.PHONY: clean
clean:
	rm *.o
	rm *.out