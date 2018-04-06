#include "./server.h"

char helpMsg[MAX];

int main(int argc, const char * argv[]) {

    int sockFD, maxfd, recentConn;
    struct sockaddr_in servAddr;

    start_server(sockFD, servAddr, argc, argv);

    fd_set readSet;

    create_connection_file(sockFD, servAddr);
    recentConn = sockFD;

    create_help_msg();

    while(1) {

        prepare_readFd(readSet, sockFD, recentConn);
        getMaxFd(maxfd, recentConn);

        if(select(maxfd + 1, &readSet, NULL, NULL, NULL) < 0)
            print_error("Select Error");

        if(FD_ISSET(sockFD, &readSet)) 
            new_connection(sockFD, recentConn);

        else {
            recentConn = sockFD;
            handle_request_from_client(sockFD, readSet);
        }
    }
    return 0;
}
