#ifndef Server_h
#define Server_h

#include "../Header.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#define CONNECTION "database/connection.bin"
#define CREDENTIAL 19
#define DATA "database/users.bin"
#define CHATROOM "database/chatrooms.bin"
#define red_bold "\33[91m\33[1m"
#define magenta_bold "\33[95m\33[1m\r"
#define helpMsgSize 362

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

extern map<string, int> chatrooms;
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
int get_file_size(string fileName);
void forward_msg(Client &clientObj, string msg);

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

// Commands
void send_chatrooms(int sockfd);
void increase_room_count(string chatroom);
void decrease_room_count(string chatroom);
void send_msg_by_name(int sockfd, string name, string msg);
void handle_command(Client &clientObj, string msg);

#endif /* Server_h */
