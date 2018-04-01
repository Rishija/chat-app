#include "../Header.h"

void Bind(int &sockfd, sockaddr_in &servAddr) {
    
    if(::bind(sockfd, (sockaddr*)&servAddr, sizeof(servAddr)) < 0)
        print_error("Bind Error");
}

void Listen(int sockfd, int backlog) {
    
    if(listen(sockfd, backlog) < 0)
        print_error("Listen Error");
}
