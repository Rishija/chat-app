#include "./server.h"

void create_connection_file(int listenFD, sockaddr_in &servAddr) {
    
    fstream fp;
    fp.open(CONNECTION, ios::out | ios::binary);
    
    Client serverListenFD(listenFD ,servAddr.sin_addr.s_addr, servAddr.sin_port);
    fp.write((char*)&serverListenFD, sizeof(serverListenFD));
    fp.close();
}

void prepare_readFd(fd_set &readSet, int sockfd) {
    
    FD_ZERO(&readSet);
    FD_SET(sockfd, &readSet);
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
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

void new_connection() {
    
    /*
     1. Accept
     2. Get client details
     3. Get username and password
     4. Verify from file
     if verified:
        5. Create `Client object`
        6. Add to `connections` file
     else
        5. Close connection
     */
    
    
    //        socklen_t len = sizeof(cliAddr);
    //        int clifd;
    //        clifd = accept(sockfd, (sockaddr*)&cliAddr, &len);
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
