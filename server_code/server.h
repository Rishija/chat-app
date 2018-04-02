#ifndef Server_h
#define Server_h

#include "../Header.h"

class Client {
public:
    in_addr_t ip;
    in_port_t port;
    char username[20];
    char chatroom[20];
};

void start_server(int &sockfd, sockaddr_in &servAddr, int argc, const char * argv[]);
void prepare_readFd(fd_set &readSet, int sockfd);
void getMaxFd(int sockfd, int &maxfd);
void new_connection();
void handle_request(sockaddr_in cliAddr);

extern map<int, Client> connections;

#endif /* Server_h */