#include "client.h"

queue<string> pending;
State state = LOGGED_OUT;

int main(int argc, const char * argv[]) {
    
    int sockfd;
    struct sockaddr_in servAddr;
    connect_to_server(sockfd, servAddr, argc, argv);

    int maxfd = sockfd + 1;
    string input;
    fd_set readSet, writeSet;

    prompt();

    while(true) {
        
        prepare_fd(readSet, writeSet, sockfd);
        
        if(select(maxfd, &readSet, &writeSet, NULL, NULL) < 0)
            print_error("Select Error");
        
        if(FD_ISSET(sockfd, &readSet)) {
            cout<<"\nIncoming from server\n";
            handle_incoming_msg(sockfd);
            prompt();
        }
        
        if(FD_ISSET(STDIN_FILENO, &readSet)) {
            getline(cin, input);
            pending.push(input);
            cout << "\n Preapred to send: " << input << "---\n\n\n";
            prompt();
        }
        
        if(FD_ISSET(sockfd, &writeSet)) {
            send_msg(sockfd);
        }
    }
    return 0;
}
