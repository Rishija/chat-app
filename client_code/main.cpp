#include "client.h"

queue<Message> pending;
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
            // cout<<"\nIncoming from server\n";
            handle_incoming_msg(sockfd);
            prompt();
        }
        
        else if(FD_ISSET(STDIN_FILENO, &readSet)) {
            string msg;
            getline(cin, msg);
            Message obj;
            strcpy(obj.message, msg.c_str());
            pending.push(obj);
            // cout << "\n Preapred to send: " << obj.message << "---\n\n\n";
            if(FD_ISSET(sockfd, &writeSet)) {
                send_msg(sockfd);
            }
            prompt();
        }
        
        if(FD_ISSET(sockfd, &writeSet)) {
            send_msg(sockfd);
        }
    }
    return 0;
}
