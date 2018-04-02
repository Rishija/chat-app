#include "../Header.h"

void connect_to_server(int &sockfd, sockaddr_in &servAddr, int argc, const char * argv[]);

int main(int argc, const char * argv[]) {
    
    int sockfd;
    struct sockaddr_in servAddr;
    connect_to_server(sockfd, servAddr, argc, argv);
    return 0;
}
