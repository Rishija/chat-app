#include "./server.h"

char helpMsg[MAX];

int main(int argc, const char * argv[]) {

    int sockFD, maxfd, recentConn, selectStatus;
    struct sockaddr_in servAddr;

    start_server(sockFD, servAddr, argc, argv);

    fd_set readSet;

    create_connection_file(sockFD, servAddr);
    recentConn = sockFD;

    create_help_msg();

    signal(SIGCHLD, [] (int signo) {
        int status, pid;
        pid = waitpid(-1, &status, 0);
        if(status != 0)
            cout << "Process " << pid << " terminated with exit status " << status << endl;
    });

    while(1) {

        prepare_readFd(readSet, sockFD, recentConn);
        getMaxFd(maxfd, recentConn);

        selectStatus = select(maxfd + 1, &readSet, NULL, NULL, NULL);

        if(selectStatus < 0) {
            if(errno != EINTR)
                print_error("Select Error");
            else
                continue;
        }

        if(FD_ISSET(sockFD, &readSet)) 
            new_connection(sockFD, recentConn);

        else {
            recentConn = sockFD;
            handle_request_from_client(sockFD, readSet);
        }
    }
    return 0;
}
