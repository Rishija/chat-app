#include "../server.h"

void handle_logout(Client &clientObj) {
    
    if(send_msg(clientObj.sockfd, "logged_out", 11)) {
        clientObj.state = LOGGED_OUT;
        forward_msg(clientObj, "I'm leaving!");
        update_client_entry(clientObj);
    }
    else
        send_msg(clientObj.sockfd, "cant_logout", 12);
}
