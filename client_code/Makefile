SHELL = /bin/bash
CC = g++
CFLAGS = -std=c++14 -c

make_client: main.o init.o ../Header.h

%.o: %.cpp ../Header.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o