#include "./server.h"

void create_connection_file(int listenFD, sockaddr_in &servAddr) {
    
    fstream fp;
    fp.open(CONNECTION, ios::out | ios::binary);
    if(!fp)
        print_error("Create connection file Error");
    
    Client serverListenFD(listenFD ,servAddr.sin_addr.s_addr, ntohs(servAddr.sin_port));
    fp.write((char*)&serverListenFD, sizeof(serverListenFD));
    fp.close();
}

void prepare_readFd(fd_set &readSet, int sockfd) {
    
    FD_ZERO(&readSet);
    FD_SET(sockfd, &readSet);
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    if(!fp)
        print_error("Prepare_readFD Error");
    
    Client obj;
    
    while(fp.tellg() != EOF) {
        fp.read((char*)&obj, sizeof(obj));
        FD_SET(obj.sockfd, &readSet);
    }
    
    fp.close();
    //    cout << "Leaving prepare_readFD.. " << endl;
}

void getMaxFd(int &maxfd) {
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    Client obj;
    maxfd = -1;
    
    while(fp.tellg() != EOF) {
        fp.read((char*)&obj, sizeof(obj));
        //        cout << "Sock: " << obj.sockfd
        //        << "\n ip: " << obj.ip
        //        << "\n port: " << obj.port
        //        << "\n username: " << obj.username
        //        << "\n chatroom: " << obj.chatroom << endl;
        maxfd = max(maxfd, obj.sockfd);
    }
    
    fp.close();
    //    cout << "Leaving getmaxfd.. " << endl;
}

bool add_client(Client &clientObj) {
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::out | ios::binary);
    if(!fp)
        return false;
    
    Client readObj;
    fp.read((char*)&readObj, sizeof(readObj));
    while(!fp.eof() && readObj.sockfd > 0)
        fp.read((char*)&readObj, sizeof(readObj));
    
    if(fp.eof())
        fp.write((char*)&clientObj, sizeof(clientObj));
    else {
        fp.seekp((long long int)fp.tellp() - sizeof(readObj), ios::beg);
        fp.write((char*)&clientObj, sizeof(clientObj));
    }
    fp.close();
    return true;
}

void new_connection(int listenFD) {
    
    sockaddr_in cliAddr;
    socklen_t len = sizeof(cliAddr);
    int conn = accept(listenFD, (sockaddr*)&cliAddr, &len);
    
    if(conn < 0)
        print_error("New connection - Accept() Error", false);
    else {
        cout << "\n" << inet_ntoa(cliAddr.sin_addr) << "\t" << ntohs(cliAddr.sin_port) << "\t" << "Connected\n";
        if(fork() == 0) {
            close(listenFD);
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
            exit(0);
        }
    }
}

void handle_request(sockaddr_in cliAddr) {
    
    /*
     1. Get request
     if successful:
        2. Process
        3. Respond
     else
        2. Close connection
        3. Remove from `connection`
     */
}
