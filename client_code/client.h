#ifndef Client_h
#define Client_h

#include "../Header.h"
#include <queue>
#define yellow "\33[93m"

enum State {LOGGED_OUT = 0, LOGGED_IN, WAITING_FOR_LOGIN_RESPONSE, WAITING_FOR_SIGNUP_RESPONSE};

void Connect(int &sockfd, sockaddr_in &servAddr);
void connect_to_server(int &sockfd, sockaddr_in &servAddr, int argc, const char * argv[]);
void prompt();
void prepare_fd(fd_set &readSet, fd_set &writeSet, int sockfd);
void handle_incoming_msg(int sockfd);
void send_msg(int sockfd);

extern State state;
extern int flags;
extern queue<string> pending;;

#endif /* Client_h */