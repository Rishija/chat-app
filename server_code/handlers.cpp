#include "server.h"

void new_connection(int listenFD, int &recentConn) {
    
    sockaddr_in cliAddr;
    socklen_t len = sizeof(cliAddr);
    int conn = accept(listenFD, (sockaddr*)&cliAddr, &len);
    
    if(conn < 0)
        print_error("New connection - Accept() Error", false);
    else {
        recentConn = conn;
        cout << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << "\t" << "Connected\n";
        if(fork() == 0) {
            close(listenFD);
            // cout<<"In fork() Trying to add to file..\n";
            // add client to `connections` file
            Client clientObj(conn, inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
            
            if(add_client(clientObj))
                        cout << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << "\t" << "Added\tFD:  "<<conn<<endl;
            else {
                cout << "Client\t"
                << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << endl;
                print_error("Connection closed. Add client to file Error", false);
            }
            char msg[] = "HOLA! WELCOME";
            if(send(conn, msg, sizeof(msg), MSG_DONTWAIT) < 0)
                print_error("Couldn't send to client", false);
            exit(0);
        }
    }
}

void handle_incoming_msg(Client obj, string msg) {
    
    cout<<"got msg: "<<msg<<endl;
    if(obj.state == LOGGED_OUT) {
        if(msg.substr(0,6) == "\\login") {
            // verify username and password
            // if(ok) change client state
        }
        else if(msg.substr(0,7) == "\\signup") {
            // validate username
            // if(ok) add username and password to users database
            // and change client state
        }
        else
            send_error_msg(obj.sockfd, "You must be logged in");
    }
}

void handle_request_from_client(int sockfd, fd_set &readSet) {
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    if(!fp)
        print_error("handle_request Error");
    
    Client obj;
    // Not to check for listen fd
    fp.read((char*)&obj, sizeof(obj));
    size_t size = get_file_size();
    // cout<<"original size: "<<size<<endl;
    
    while((size_t)fp.tellg() < size) {
        
        // cout<<"fp: "<<(int)fp.tellg()<<endl;
        fp.read((char*)&obj, sizeof(obj));
        
        if((obj.sockfd > 0) && FD_ISSET(obj.sockfd, &readSet)) {
            
            char msg[MAX];
            int recvStatus = recv(obj.sockfd, msg, MAX, MSG_DONTWAIT);
            cout << "\nReceived: "<<recvStatus<<" from fd: "<<obj.sockfd<<" port: "<<ntohs(obj.port)
            <<"msg: "<<msg<<endl;
            
            if(recvStatus < 0 && (recvStatus == EAGAIN || recvStatus == EWOULDBLOCK) )
                print_error("Try receiving after some time", false);
            else if(recvStatus <= 0 || !strcmp(msg, "\\quit")) {
                close(obj.sockfd);
                remove_client(obj.sockfd);
                cout << obj.ip << "\t" << htons(obj.port) << "\t" << "Disconnected\n";
                // To do: send exit of client to his chatroom
                return;
            }
            // if(fork() == 0) {
            // close(sockfd);
            handle_incoming_msg(obj, msg);
            // }
        }
    }
    
    fp.close();
}