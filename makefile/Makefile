CC = g++
CFLAGS = -Wall -std=c++1z -w -L. -I.
OBJ = test.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

hello: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(OBJ) test