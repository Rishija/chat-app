SHELL = /bin/bash
CC = g++
CFLAGS = -std=c++14
SERVER_CODES := $(wildcard server_code/*.cpp)
SERVER_OBJECTS := $(SERVER_CODES:.cpp=.o)

SERVER_MODULES := $(wildcard server_code/modules/*.cpp)
SERVER_MODULES_OBJECTS := $(SERVER_MODULES:.cpp=.o)

CLIENT_CODES := $(wildcard client_code/*.cpp)
CLIENT_OBJECTS := $(CLIENT_CODES:.cpp=.o)

all: make_server make_client

make_server:
	cd server_code; make
	$(CC) $(CFLAGS) $(SERVER_OBJECTS) $(SERVER_MODULES_OBJECTS) -o server

make_client:
	cd client_code; make
	$(CC) $(CFLAGS) $(CLIENT_OBJECTS) -o client

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.bin' -delete
	rm server client
