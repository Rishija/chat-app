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
using namespace std;

#define PORT 5555
#define BACKLOG 20
#define red "\33[91m"
#define green "\33[92m"
#define bold "\33[1m"
#define regular "\33[0m"

void print_error(string err = "");
void Socket(int &sockfd);
void Bind(int &sockfd, sockaddr_in &servAddr);
void Listen(int sockfd, int backlog);
void Connect(int &sockfd, sockaddr_in &servAddr);

inline void print_error(string err) {
    
    if(err != "")
        cout << red << err << regular << endl;
    
    if(errno){
        cout << red << strerror(errno) << regular << endl;
        exit(errno);
    }
}

inline void Socket(int &sockfd) {
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd < 0 ? print_error("Socket Error") : print_error();
}

#endif /* Header_h */
