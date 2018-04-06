#include "client.h"

void prompt() {
    
    cout << yellow << bold << "\n";
    (state == LOGGED_IN || state == WAITING_FOR_LOGOUT_RESPONSE)
    ? cout << "You: " : cout << "> ";
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

void handle_incoming_msg(int sockfd) {
    
    char msg[MAX];
    int recvStatus = recv(sockfd, msg, MAX, MSG_DONTWAIT);
    // cout<<"received: "<<msg<<endl;
    
    if(recvStatus < 0) {
        if(recvStatus == EAGAIN || recvStatus == EWOULDBLOCK)
            print_error("\rTry receiving after some time", false);
        else {
            errno = recvStatus;
            print_error("\rCan't read from server");
        }
    }
    if(recvStatus == 0) {
        print_error("\rConnection Closed From Server\n", false);
        exit(0);
    }
    else {
        if(state == WAITING_FOR_LOGIN_RESPONSE) {
            if(!strcmp(msg, "logged_in")) {
                state = LOGGED_IN;
                cout << green << bold << "\rLogged in\n" << regular;
            }
            else if(!strcmp(msg, "mismatch")) {
                state = LOGGED_OUT;
                print_error("\rUsername and password doesn't match", false);
            }
            else if(!strcmp(msg, "ill_format")) {
                state = LOGGED_OUT;
                print_error("\rEnter valid username and password", false);
            }
        }
        else if(state == WAITING_FOR_SIGNUP_RESPONSE) {
            if(!strcmp(msg, "signed_up")) {
                state = LOGGED_IN;
                cout << green << bold << "\rSigned up and logged in\n" << regular;
            }
            else if(!strcmp(msg, "username_taken")) {
                state = LOGGED_OUT;
                print_error("\rUsername already taken", false);
            }
            else if(!strcmp(msg, "ill_format")) {
                state = LOGGED_OUT;
                print_error("\rEnter valid username and password", false);
            }
        }
        else if(state == WAITING_FOR_LOGOUT_RESPONSE) {
            if(!strcmp(msg, "logged_out")) {
                state = LOGGED_OUT;
                cout << green << bold << "\rLogged out\n" << regular;
            }
            else if(!strcmp(msg, "cant_logout")) {
                state = LOGGED_IN;
                print_error("\rCant't Log out, Try again later", false);
            }
            else
                cout << msg;
        }
        else
            cout << msg;
    }
}

void send_msg(int sockfd) {
    
    if(pending.empty())
        return;
    
    Message input = pending.front();
    pending.pop();
    string input1 = input.message;
    // cout << "\nsending to server: " <<input.message<<endl;

    if(send_msg(sockfd, input.message, input1.size() + 1)) {

        if(input1 == "\\logout" && state == LOGGED_IN) {
            state = WAITING_FOR_LOGOUT_RESPONSE;
        }

        else if(input1.substr(0, 6) == "\\login") {
            state = WAITING_FOR_LOGIN_RESPONSE;
        }
        else if(input1.substr(0, 7) == "\\signup") {
            state = WAITING_FOR_SIGNUP_RESPONSE;
        }
    }
    // cout<<"\nsent\n";
}
