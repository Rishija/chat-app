#include "client.h"

queue<Message> pending;
State state = LOGGED_OUT;

int main(int argc, const char * argv[]) {

    int sockFD;
    struct sockaddr_in servAddr;
    connect_to_server(sockFD, servAddr, argc, argv);

    int maxfd = sockFD + 1;
    string input;
    fd_set readSet, writeSet;

    prompt();

    while(true) {

        prepare_fd(readSet, writeSet, sockFD);

        if(select(maxfd, &readSet, &writeSet, NULL, NULL) < 0)
            print_error("Select Error");

        // Server is sending message
        if(FD_ISSET(sockFD, &readSet)) {
            handle_incoming_msg(sockFD);
            prompt();
        }

        // Input from stdin
        else if(FD_ISSET(STDIN_FILENO, &readSet)) {

            // Get input from stdin
            string msg;
            getline(cin, msg);
            Message obj;
            strcpy(obj.message, msg.c_str());
            // Push in queue
            pending.push(obj);

            // Server ready to receive
            if(FD_ISSET(sockFD, &writeSet)) {
                send_msg(sockFD);
            }
            prompt();
        }

        else if(FD_ISSET(sockFD, &writeSet)) {
            send_msg(sockFD);
        }
    }
    return 0;
}
