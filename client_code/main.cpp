#include "../Header.h"

int main(int argc, const char * argv[]) {
    
    int sockfd;
    struct sockaddr_in servAddr;
    connect_to_server(sockfd, servAddr, argc, argv);
    return 0;
}
