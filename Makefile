SHELL = /bin/bash
CC = g++
CFLAGS = -std=c++14
SERVER_CODES := $(wildcard server_code/*.cpp)
SERVER_OBJECTS := $(SERVER_CODES:.cpp=.o)
CLIENT_CODES := $(wildcard client_code/*.cpp)
CLIENT_OBJECTS := $(CLIENT_CODES:.cpp=.o)

all: 
	cd server_code; make
	cd ..
	$(CC) $(CFLAGS) $(SERVER_OBJECTS) -o server
	cd client_code; make
	cd ..
	$(CC) $(CFLAGS) $(CLIENT_OBJECTS) -o client

clean:
	cd server_code; make clean
	cd client_code; make clean
	rm server client