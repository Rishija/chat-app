#include "client.h"

void prompt() {
    
    cout << yellow << bold << "\n";
    (state == LOGGED_OUT) ? cout << "> " : cout << "You: ";
    cout << regular;
    cout << flush;
}

void prepare_fd(fd_set &readSet, fd_set &writeSet, int sockfd) {
    
    FD_ZERO(&readSet);
    FD_SET(sockfd, &readSet);
    FD_SET(STDIN_FILENO, &readSet);
    
    FD_ZERO(&writeSet);
    FD_SET(sockfd, &writeSet);
}

void handle_incoming_msg() {cout<<"Incoming from server";}

void send_msg() {cout<<"Sending msg\n";}