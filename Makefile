SHELL = /bin/bash
CC = g++
CFLAGS = -std=c++14
SERVER_CODES := $(wildcard server_code/*.cpp)
SERVER_OBJECTS := $(SERVER_CODES:.cpp=.o)

all: 
	cd server_code; make
	$(CC) $(CFLAGS) $(SERVER_OBJECTS) -o server

clean:
	cd server_code; make clean
	rm server