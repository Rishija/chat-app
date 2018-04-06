#ifndef Server_h
#define Server_h

#include "../Header.h"
#include <fstream>
#include <sstream>
#include <vector>
#define CONNECTION "connection.bin"
#define CREDENTIAL 19
#define DATA "data.bin"
#define red_bold "\33[91m\33[1m"
#define helpMsgSize 352

struct Client {
public:
    
    Client(int clientSockfd = -1, const char clientIp[16] = "", unsigned short clientPORT = 0, const char clientName[20] = "", const char clientRoom[20] = "") :
    sockfd(clientSockfd), port(htons(clientPORT)){
        state = LOGGED_OUT;
        strcpy(ip, clientIp);
        strcpy(username, clientName);
        strcpy(chatroom, clientRoom);
    }
    int sockfd;
    char ip[16];
    uint16_t port;
    State state;
    char username[20];
    char chatroom[20];
};

extern char helpMsg[MAX];

// Starting server
void Bind(int &sockfd, sockaddr_in &servAddr);
void Listen(int sockfd, int backlog);
void start_server(int &sockfd, sockaddr_in &servAddr, int argc, const char * argv[]);

// Helpers
void create_help_msg();
void prepare_readFd(fd_set &readSet, int sockfd, int recentConn);
void getMaxFd(int &maxfd, int recentConn);
void create_connection_file(int listenFD, sockaddr_in &servAddr);
bool add_client(Client &obj);
void remove_client(int clientFD);
void send_error_msg(int clientFD, string msg);
void print_obj(Client &obj);
int get_file_size();

// Handlers
void update_client_entry(Client &clientObj);
void new_connection(int sockfd, int &recentConn);
void handle_incoming_msg(Client &clientObj, string msg);
void handle_request_from_client(int sockfd, fd_set &readSet);

// Login
bool credentials_ok(const char *name, const char *pass);
void handle_login(Client &clientObj, const string &msg);

// Signup
bool valid_username(const char *name);
bool add_to_data(const char *name, const char *pass);
void handle_signup(Client &clientObj, const string &msg);

// Logout
void handle_logout(Client &clientObj);

#endif /* Server_h */
