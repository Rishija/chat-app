#ifndef Client_h
#define Client_h

#include "../Header.h"
#include <cstdio>

#define yellow "\33[93m"

enum State {LOGGED_OUT = 0, LOGGED_IN};

void Connect(int &sockfd, sockaddr_in &servAddr);
void connect_to_server(int &sockfd, sockaddr_in &servAddr, int argc, const char * argv[]);
void prompt();
void prepare_fd(fd_set &readSet, fd_set &writeSet, int sockfd);
void handle_incoming_msg();
void send_msg(); 

extern State state;

#endif /* Client_h */