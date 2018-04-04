#include "server.h"

void new_connection(int listenFD) {
    
    sockaddr_in cliAddr;
    socklen_t len = sizeof(cliAddr);
    int conn = accept(listenFD, (sockaddr*)&cliAddr, &len);
    
    if(conn < 0)
        print_error("New connection - Accept() Error", false);
    else {
        cout << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << "\t" << "Connected\n";
        // if(fork() == 0) {
            // close(listenFD);
            // cout<<"In fork() Trying to add to file..\n";
            // add client to `connections` file
            Client clientObj(conn, cliAddr.sin_addr.s_addr, ntohs(cliAddr.sin_port));
            if(add_client(clientObj))
                cout << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << "\t" << "Added\n";
            else {
                cout << "Client\t"
                << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << endl;
                close(conn);
                print_error("Connection closed. Add client to file Error", false);
            }
            // exit(0);
        // }
    }
}

void handle_incoming_msg(Client obj, string msg) {
    
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
    
    while(fp.tellg() != EOF) {
        
        fp.read((char*)&obj, sizeof(obj));
        if(FD_ISSET(obj.sockfd, &readSet)) {
        	cout << "Found set for: " << obj.ip << "\t" << obj.port;
            string msg;
            int received = recv(obj.sockfd, (char*)&msg, MAX, 0);
            cout << "\n received: "<<received<<endl;
            if(received <= 0 || msg == "\\quit") {
                close(obj.sockfd);
                remove_client(obj.sockfd);
                cout << obj.ip << "\t" << obj.port << "\t" << "Disconnected\n";
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
