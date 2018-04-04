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

void remove_client(int clientFD) {
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::out | ios::binary);
    
    Client readObj, nullObj;
    fp.read((char*)&readObj, sizeof(readObj));
    while(readObj.sockfd != clientFD)
        fp.read((char*)&readObj, sizeof(readObj));
    
    fp.seekp((long long int)fp.tellp() - sizeof(readObj), ios::beg);
    fp.write((char*)&nullObj, sizeof(nullObj));
    fp.close();
}


void send_error_msg(int clientFD, string msg) {
    
    string newMsg = red_bold + msg + regular;
    if(send(clientFD, (char*)&newMsg, sizeof(newMsg), MSG_DONTWAIT) < 0)
        print_error("send_error_msg to client Error");
}
