#include "./server.h"

int main(int argc, const char * argv[]) {

    int sockfd, maxfd;
    struct sockaddr_in servAddr, cliAddr;

    start_server(sockfd, servAddr, argc, argv);

    fd_set readSet;

    create_connection_file(sockfd, servAddr);

    while(1) {

        prepare_readFd(readSet, sockfd);
        getMaxFd(maxfd);
//        cout << "\nAfter fn call max: " << maxfd << endl;

        if(select(maxfd + 1, &readSet, NULL, NULL, NULL) < 0)
            print_error("Select Error");

        if(FD_ISSET(sockfd, &readSet))
            new_connection();

        // open `connection` file and compare
        // for(auto sock : connections)
        //     if(FD_ISSET(sock.first, &readSet))
        //         handle_request(cliAddr);
    }
    return 0;
}
