#ifndef Header_h
#define Header_h

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <map>
#include <sys/select.h>
#include <unistd.h>
using namespace std;

#define PORT 5555
#define BACKLOG 20
#define MAX 2048
#define red "\33[91m"
#define green "\33[92m"
#define bold "\33[1m"
#define regular "\33[0m"

enum State {LOGGED_OUT = 0,
    LOGGED_IN,
    WAITING_FOR_LOGIN_RESPONSE,
    WAITING_FOR_SIGNUP_RESPONSE,
    WAITING_FOR_LOGOUT_RESPONSE };


void print_error(string err = "", bool shouldExit = true);
void Socket(int &sockFD);
bool send_msg(int clientFd, const char *msg, const size_t size);

// Print error with message on stdin
inline void print_error(string err, bool shouldExit) {
    
    if(err != "")
        cout << red <<bold << err << regular;

    if(errno){
        cout << " -> " << red << strerror(errno) << regular << endl;
        if(shouldExit)
            exit(errno);
    }
}

// Creates a sock fd
inline void Socket(int &sockFD) {
    
    sockFD = socket(AF_INET, SOCK_STREAM, 0);
    sockFD < 0 ? print_error("Socket Error") : print_error();
}

// Send message to sockFD
inline bool send_msg(int sockFD, const char *msg, const size_t size) {

    int sendStatus = send(sockFD, msg, size, MSG_DONTWAIT);
    if(sendStatus < 0) {
        cout<<"\nerr\n";
        if(sendStatus == EMSGSIZE) {
            print_error("\rMessage size too large", false);
            return false;
        }
        else if(sendStatus == EAGAIN || sendStatus == EWOULDBLOCK) {
            print_error("\rTry after some time", false);
            return false;
        }
        else {
            errno = sendStatus;
            print_error("\rSend message Error");
        }
    }
    return true;
}

#endif /* Header_h */
