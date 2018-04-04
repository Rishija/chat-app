SHELL = /bin/bash
CC = g++
CFLAGS = -std=c++14 -c
DEP = server.h ../Header.h

make_server: main.o init.o helpers.o handlers.o $(DEP)

%.o: %.cpp $(DEP)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o *.out
