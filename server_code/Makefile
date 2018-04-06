SHELL = /bin/bash
CC = g++
CFLAGS = -std=c++14 -c
DEP = server.h ../Header.h
OBJ = main.o modules/login.o modules/logout.o modules/signup.o init.o helpers.o handlers.o
make_server: $(OBJ) $(DEP)

%.o: %.cpp $(DEP)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm $(OBJ)
