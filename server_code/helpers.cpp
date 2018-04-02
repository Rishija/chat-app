#include "./server.h"

void prepare_readFd(fd_set &readSet, int sockfd) {
    
    FD_ZERO(&readSet);
    FD_SET(sockfd, &readSet);
    for(auto sock : connections)
        FD_SET(sock.first, &readSet);
}

void getMaxFd(int sockfd, int &maxfd) {
    
    maxfd = sockfd;
    for(auto sock : connections)
        maxfd = max(maxfd, sock.first);
}

void new_connection() {
    
    /*
     1. Accept
     2. Get client details
     3. Get username and password
     4. Verify from file
     if verified:
        5. Create `Client object`
        6. Add to `connections`
     else
        5. Close connection
     */
    
    
    //        socklen_t len = sizeof(cliAddr);
    //        int clifd;
    //        clifd = accept(sockfd, (sockaddr*)&cliAddr, &len);
}

void handle_request(sockaddr_in cliAddr) {
    
    /*
     1. Get request
     if successful:
        2. Process
        3. Respond
     else
        2. Close connection
        3. Remove from `connection`
     */
}
