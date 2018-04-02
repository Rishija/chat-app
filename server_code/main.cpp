#include "./server.h"

map<int, Client> connections;

int main(int argc, const char * argv[]) {
    
    int sockfd, maxfd;
    struct sockaddr_in servAddr, cliAddr;
    
    start_server(sockfd, servAddr, argc, argv);
    
    fd_set readSet;
    
    while(1) {
        
        prepare_readFd(readSet, sockfd);
        getMaxFd(sockfd, maxfd);
        
        if(select(maxfd, &readSet, NULL, NULL, NULL) < 0)
            print_error("Select Error");
        
        if(FD_ISSET(sockfd, &readSet))
            new_connection();
        
        for(auto sock : connections)
            if(FD_ISSET(sock.first, &readSet))
                handle_request(cliAddr);
    }
    return 0;
}
