#include "client.h"

void prompt() {
    
    cout << yellow << bold ;
    (state == LOGGED_IN || state == WAITING_FOR_LOGOUT_RESPONSE)
    ? cout << "You: " : cout << "> ";
    cout << regular;
    cout << flush;
}


void prepare_fd(fd_set &readSet, fd_set &writeSet, int sockFD) {

    // Set server socket and stdin for read fd set
    FD_ZERO(&readSet);
    FD_SET(sockFD, &readSet);
    FD_SET(STDIN_FILENO, &readSet);

    // Set server socket for write fd set
    FD_ZERO(&writeSet);
    FD_SET(sockFD, &writeSet);
}


void handle_incoming_msg(int sockFD) {

    char msg[MAX];
    int recvStatus = recv(sockFD, msg, MAX, MSG_DONTWAIT);

    if(recvStatus < 0) {
        if(recvStatus == EAGAIN || recvStatus == EWOULDBLOCK)
            print_error("\rTry receiving after some time\n", false);
        else {
            errno = recvStatus;
            print_error("\rCan't read from server\n");
        }
    }

    if(recvStatus == 0) {
        print_error("\rConnection Closed From Server\n", false);
        exit(0);
    }

    else {
        // Login message has been sent, waiting for response
        if(state == WAITING_FOR_LOGIN_RESPONSE) {
            if(!strcmp(msg, "logged_in")) {
                state = LOGGED_IN;
                cout << green << bold << "\rLogged in\n" << regular;
            }

            else if(!strcmp(msg, "mismatch")) {
                state = LOGGED_OUT;
                print_error("\rUsername and password doesn't match\n", false);
            }

            else if(!strcmp(msg, "ill_format")) {
                state = LOGGED_OUT;
                print_error("\rEnter valid username and password\n", false);
            }
        }

        // Signup message has been sent, waiting for response
        else if(state == WAITING_FOR_SIGNUP_RESPONSE) {
            if(!strcmp(msg, "signed_up")) {
                state = LOGGED_IN;
                cout << green << bold << "\rSigned up and logged in\n" << regular;
            }

            else if(!strcmp(msg, "username_taken")) {
                state = LOGGED_OUT;
                print_error("\rUsername already taken\n", false);
            }

            else if(!strcmp(msg, "ill_format")) {
                state = LOGGED_OUT;
                print_error("\rEnter valid username and password\n", false);
            }
        }

        // Logout message has been sent, waiting for response
        else if(state == WAITING_FOR_LOGOUT_RESPONSE) {
            if(!strcmp(msg, "logged_out")) {
                state = LOGGED_OUT;
                cout << green << bold << "\rLogged out\n" << regular;
            }

            else if(!strcmp(msg, "cant_logout")) {
                state = LOGGED_IN;
                print_error("\rCant't Log out, Try again later\n", false);
            }

            // Some incoming message
            else
                cout << msg;
        }

        // Some incoming message
        else
            cout << msg;
    }
}


void send_msg(int sockFD) {
    
    // All messages have been sent to the server
    if(pending.empty())
        return;

    // Get the first pending message
    Message input = pending.front();
    pending.pop();
    string input1 = input.message;

    if(send_msg(sockFD, input.message, input1.size() + 1)) {

        if(input1 == "\\logout" && state == LOGGED_IN) {
            state = WAITING_FOR_LOGOUT_RESPONSE;
        }

        else if(input1.substr(0, 7) == "\\login " && state == LOGGED_OUT) {
            state = WAITING_FOR_LOGIN_RESPONSE;
        }

        else if(input1.substr(0, 8) == "\\signup " && state == LOGGED_OUT) {
            state = WAITING_FOR_SIGNUP_RESPONSE;
        }
    }
}
