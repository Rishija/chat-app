#ifndef Client_h
#define Client_h

#include "../Header.h"
#include <queue>
#define yellow "\33[93m"

struct Message {
    char message[MAX];
};

void Connect(int &sockFD, sockaddr_in &servAddr);
void connect_to_server(int &sockFD, sockaddr_in &servAddr, int argc, const char * argv[]);
void prompt();
void prepare_fd(fd_set &readSet, fd_set &writeSet, int sockFD);
void handle_incoming_msg(int sockFD);
void send_msg(int sockFD);

extern State state;
extern queue<Message> pending;

#endif /* Client_h */
