#include "./server.h"

char helpMsg[MAX];

int main(int argc, const char * argv[]) {

    int sockfd, maxfd, recentConn;
    struct sockaddr_in servAddr;
    
    start_server(sockfd, servAddr, argc, argv);
    
    fd_set readSet;
    
    create_connection_file(sockfd, servAddr);
    recentConn = sockfd;

    create_help_msg();
    
    while(1) {
        
        prepare_readFd(readSet, sockfd, recentConn);
        getMaxFd(maxfd, recentConn);

        cout << "\nAfter fn call max: " << maxfd << endl;
        
        if(select(maxfd + 1, &readSet, NULL, NULL, NULL) < 0)
            print_error("Select Error");
        
        
        if(FD_ISSET(sockfd, &readSet)) 
            new_connection(sockfd, recentConn);
        
        else {
            recentConn = sockfd;
            cout<<"\nRequest coming..";
            handle_request_from_client(sockfd, readSet);
            cout<<"returned from handle request from client\n";
        }
    }
    return 0;
}
