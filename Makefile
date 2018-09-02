CC = g++
CFLAGS = -Wall -std=c++1z -w -L. -I.
DEPS = header.h abc.h
OBJ = header.o Ex.o abc.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

hello: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(OBJ)