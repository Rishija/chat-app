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

void print_error(string err = "", bool shouldExit = true);
void Socket(int &sockfd);

inline void print_error(string err, bool shouldExit) {
    
    if(err != "")
        cout << red << err << regular;

    if(errno){
        cout << " -> " << red << strerror(errno) << regular << endl;
        if(shouldExit)
            exit(errno);
    }
}

inline void Socket(int &sockfd) {
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd < 0 ? print_error("Socket Error") : print_error();
}

#endif /* Header_h */
