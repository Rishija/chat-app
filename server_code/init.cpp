#include "../Header.h"

void print_error(string err) {
    
    if(err != "")
        cout << red << err << regular << endl;
    
    if(errno){
        cout << red << strerror(errno) << regular << endl;
        exit(errno);
    }
}

void Socket(int &sockfd) {
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd < 0 ? print_error("Socket Error") : print_error();
}

void Bind(int &sockfd, sockaddr_in &servAddr) {
    
    if(::bind(sockfd, (sockaddr*)&servAddr, sizeof(servAddr)) < 0)
        print_error("Bind Error");
}

void Listen(int sockfd, int backlog) {
    
    if(listen(sockfd, backlog) < 0)
        print_error("Listen Error");
}
