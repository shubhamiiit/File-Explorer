CC=g++

CFLAGS=-c -Wall

all: hello

hello: Ex.o abc.o
		$(CC) Ex.o abc.o -o hello

Ex.o: Ex.cpp
		$(CC) $(CFLAGS) Ex.cpp

abc.o: abc.cpp
		$(CC) $(CFLAGS) Ex.cpp

clean: 
		rm -rf *o hello