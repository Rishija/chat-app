#include "../server.h"

void handle_logout(Client &clientObj) {

    if(send_msg(clientObj.sockFD, "logged_out", 11)) {
        clientObj.state = LOGGED_OUT;
        // Inform everybody in client's chatroom about client's exit
        forward_msg(clientObj, "LEFT", true);
        // Update user state
        update_client_entry(clientObj);
    }
    else
        send_msg(clientObj.sockFD, "cant_logout", 12);
}
