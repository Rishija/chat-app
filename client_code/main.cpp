#include "client.h"

State state = LOGGED_OUT;

int main(int argc, const char * argv[]) {
    
    int sockfd;
    struct sockaddr_in servAddr;
    connect_to_server(sockfd, servAddr, argc, argv);

    int maxfd = sockfd + 1;
    string input;
    State state = LOGGED_OUT;
    fd_set readSet, writeSet;

    prompt();

    while(true) {

    	prepare_fd(readSet, writeSet, sockfd);

    	if(select(maxfd, &readSet, &writeSet, NULL, NULL) < 0)
    		print_error("Select Error");

    	if(FD_ISSET(sockfd, &readSet)) {
    		handle_incoming_msg();
	    	prompt();
    	}

    	if(FD_ISSET(STDIN_FILENO, &readSet)) {
    		getline(cin, input);
    		send_msg();
    		prompt();
    	}
    }
    return 0;
}
