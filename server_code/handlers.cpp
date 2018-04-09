#include "server.h"

void new_connection(int listenFD, int &recentConn) {

    sockaddr_in cliAddr;
    socklen_t len = sizeof(cliAddr);
    int conn = accept(listenFD, (sockaddr*)&cliAddr, &len);

    if(conn < 0)
        print_error("New connection - Accept() Error", false);

    else {
        // Update recent connection
        recentConn = conn;
        cout << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << "\t" << "Connected\n";

        if(fork() == 0) {

            close(listenFD);
            Client clientObj(conn, inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

            // add client to `connections` file
            if(add_client(clientObj))
                cout << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << "\t" << "Added\t\tFD:  "<<conn<<endl;

            else {
                cout << "Client\t"
                << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << endl;
                print_error("Connection closed. Add client to file Error", false);
            }

            // Connection accepted and written in file. Send help msg to client
            send_msg(conn, helpMsg, helpMsgSize);
            exit(0);
        }
    }
}


void handle_incoming_msg(Client &clientObj, string msg) {

    if(msg.size() == 0)
        return;

    // cout<<"handle_incoming_msg() got msg: "<<msg<<endl;
    if(msg == "\\help") {
        send_msg(clientObj.sockFD, helpMsg, helpMsgSize);
        return;
    }

    // If client is logged out only valid action is login and signup
    if(clientObj.state == LOGGED_OUT) {

        if(msg.substr(0,6) == "\\login") {
            // cout <<"Calling handle_login().. \n";
            handle_login(clientObj, msg);
        }

        else if(msg.substr(0,7) == "\\signup") {
            // cout <<"Calling handle_signup().. \n";
            handle_signup(clientObj, msg);
        }

        else {
            // cout <<"Sending must be logged in..\n";
            send_error_msg(clientObj.sockFD, "\rYou must be logged in\n");
        }
        return;
    }

    // User is logged_in
    // Incoming message is a command
    if(msg[0] == '\\')
        handle_other_commands(clientObj, msg);
    // Forward message to client's chatroom
    else
        forward_msg(clientObj, msg);
}


void handle_request_from_client(int sockFD, fd_set &readSet) {
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    if(!fp)
        print_error("handle_request Error\n");

    Client obj;
    // Not to check for listen fd
    fp.read((char*)&obj, sizeof(obj));
    size_t size = get_file_size(CONNECTION);

    while((size_t)fp.tellg() < size) {

        fp.read((char*)&obj, sizeof(obj));

        // Client fd is ready to be read
        if((obj.sockFD > 0) && FD_ISSET(obj.sockFD, &readSet)) {

            char msg[MAX];
            int recvStatus = recv(obj.sockFD, msg, MAX, MSG_DONTWAIT);
            // cout << "\nReceived: "<<recvStatus<<" from fd: "<<obj.sockFD<<" port: "<<ntohs(obj.port)
            // <<" msg: "<<msg<<endl;

            if(recvStatus < 0 && (recvStatus == EAGAIN || recvStatus == EWOULDBLOCK) )
                print_error("Try receiving after some time\n", false);

            // Client connection closed or client wants to exit
            else if(recvStatus <= 0 || !strcmp(msg, "\\quit")) {
                close(obj.sockFD);
                // Send leaving to client's chatroom
                forward_msg(obj, "I'm leaving!");
                // Remove from connection file
                remove_client(obj.sockFD);
                // Decrease count of chatroom client was member of
                decrease_room_count(obj.chatroom);
                cout << obj.ip << "\t" << htons(obj.port) << "\t" << "Disconnected\n";
                return;
            }
            if(fork() == 0) {
                close(sockFD);
                // cout <<"fork() handling the command..\n";
                handle_incoming_msg(obj, msg);
                exit(0);
            }
        }
    }

    fp.close();
}
