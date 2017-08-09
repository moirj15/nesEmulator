CC = g++
FLAGS = -std=c++14 -Wall -Wextra -pedantic
OBJ = main.o 6502_cpu.o debug.o 
FILES = $*.c $*.h

all: $(OBJ)
	$(CC) $(OBJ) -lSDL2 -o main

$.c: $(FILES)
	$(CC) -c $(FILES)

clean:
	rm *o main
