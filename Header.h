#ifndef Header_h
#define Header_h

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
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

#endif /* Header_h */
