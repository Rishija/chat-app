#ifndef Client_h
#define Client_h

#include "../Header.h"
#include <queue>
#define yellow "\33[93m"

void Connect(int &sockfd, sockaddr_in &servAddr);
void connect_to_server(int &sockfd, sockaddr_in &servAddr, int argc, const char * argv[]);
void prompt();
void prepare_fd(fd_set &readSet, fd_set &writeSet, int sockfd);
void handle_incoming_msg(int sockfd);
void send_msg(int sockfd);

extern State state;
extern queue<Message> pending;;

#endif /* Client_h */
