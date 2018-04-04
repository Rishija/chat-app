#ifndef Server_h
#define Server_h

#include "../Header.h"
#include <fstream>
#define CONNECTION "connection.bin"
#define red_bold "\33[91m\33[1m"

struct Client {
public:
    
    Client(int clientSockfd = -1, unsigned long clientIP = 0, unsigned short clientPORT = 0, const char clientName[20] = "", const char clientRoom[20] = "") :
    sockfd(clientSockfd), ip(clientIP), port(htons(clientPORT)){
        state = LOGGED_OUT;
        strcpy(username, clientName);
        strcpy(chatroom, clientRoom);
    }
    int sockfd;
    unsigned long ip;
    uint16_t port;
    State state;
    char username[20];
    char chatroom[20];
};

// Starting server
void Bind(int &sockfd, sockaddr_in &servAddr);
void Listen(int sockfd, int backlog);
void start_server(int &sockfd, sockaddr_in &servAddr, int argc, const char * argv[]);

// Helpers
void prepare_readFd(fd_set &readSet, int sockfd);
void getMaxFd(int &maxfd);
void create_connection_file(int listenFD, sockaddr_in &servAddr);
bool add_client(Client &obj);
void remove_client(int clientFD);
void send_error_msg(int clientFD, string msg);

// Handlers
void new_connection(int sockfd);
void handle_incoming_msg(Client obj, string msg);
void handle_request_from_client(int sockfd, fd_set &readSet);

#endif /* Server_h */
