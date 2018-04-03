#ifndef Server_h
#define Server_h

#include "../Header.h"
#include <fstream>
#define CONNECTION "connection.bin"

struct Client {
public:

    Client(int clientSockfd = -1, unsigned long clientIP = 0, unsigned short clientPORT = 0, const char clientName[20] = "", const char clientRoom[20] = "") :
    sockfd(clientSockfd), ip(clientIP), port(htons(clientPORT)){
        strcpy(username, clientName);
        strcpy(chatroom, clientRoom);
    }
    int sockfd;
    unsigned long ip;
    uint16_t port;
    char username[20];
    char chatroom[20];
};

void start_server(int &sockfd, sockaddr_in &servAddr, int argc, const char * argv[]);
void create_connection_file(int listenFD, sockaddr_in &servAddr);
void prepare_readFd(fd_set &readSet, int sockfd);
void getMaxFd(int &maxfd);
void new_connection();
void handle_request(sockaddr_in cliAddr);


#endif /* Server_h */