#include "./server.h"

void create_help_msg() {

    strcat(helpMsg, green);
     // strcat(helpMsg, bold);
    strcat(helpMsg,
        "\r\\signup user pass\n\tTo signup with username = user and password = pass\n"
        "\\login user pass\n\tTo login with username = user and password = pass\n"
        "\\send other msg\n\tTo send personal message to client = other\n"
        "\\join room\n\tTo join or create new chatroom = room\n"
        "\\chatroom\n\tTo view current chatrooms\n"
        "\\help\n\tTo see this help message\n"
        "\\logout\n\tTo logout\n"
        "\\quit\n\tTo exit\n");
    strcat(helpMsg, regular);
}

int get_file_size(string fileName) {
    
    // cout<<"Checking size for: "<<fileName<<endl;
    fstream tempFP;
    tempFP.open(fileName, ios::app | ios::binary);
    if(!tempFP)
        print_error("Error opening connection file");
    size_t size = (size_t)tempFP.tellp();
    tempFP.close();
    // cout<<"Size calculated: "<<size<<endl;
    return size;
}

void print_obj(Client &obj) {
    
    cout << "Sock: " << obj.sockfd
    << "\n ip: " << obj.ip
    << "\n port: " << ntohs(obj.port)
    << "\n username: " << obj.username
    << "\n chatroom: " << obj.chatroom << endl;
}

void create_connection_file(int listenFD, sockaddr_in &servAddr) {
    
    fstream fp;
    fp.open(CONNECTION, ios::out | ios::binary);
    if(!fp)
        print_error("Create connection file Error");
    
    Client serverListenFD(listenFD, inet_ntoa(servAddr.sin_addr), ntohs(servAddr.sin_port), "server");
    fp.write((char*)&serverListenFD, sizeof(serverListenFD));
    // cout<<"\nCREATED\n";
    fp.close();
}

void prepare_readFd(fd_set &readSet, int sockfd, int recentConn) {
    
    FD_ZERO(&readSet);
    FD_SET(sockfd, &readSet);
    FD_SET(recentConn, &readSet);
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    if(!fp)
        print_error("Prepare_readFD Error");
    
    Client obj;
    size_t size = get_file_size(CONNECTION);
    
    while((size_t)fp.tellg() < size) {
        fp.read((char*)&obj, sizeof(obj));
        // cout<<"FD_SET for : ";
        // print_obj(obj);
        if(obj.sockfd != -1)
            FD_SET(obj.sockfd, &readSet);
    }
    
    fp.close();
    // cout << "Leaving prepare_readFD.. " << endl;
}

void getMaxFd(int &maxfd, int recentConn) {
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    Client obj;
    maxfd = recentConn;
    int size = get_file_size(CONNECTION);
    
    while((int)fp.tellg() < size) {
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
    
    // cout << "\nIn add_client.. \nWriting this obj.."<<endl;
    // print_obj(clientObj);
    // cout <<"\nData in file..\n";
    
    size_t size = get_file_size(CONNECTION);
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::out | ios::binary);
    if(!fp)
        return false;
    
    Client readObj(1);
    while((size_t)fp.tellg() < size && readObj.sockfd > 0) {
        fp.read((char*)&readObj, sizeof(readObj));
        // cout<<"\n---read---\n";
        // print_obj(readObj);
    }
    
    if(readObj.sockfd < 0) {
        fp.seekp((size_t)fp.tellg() - sizeof(readObj), ios::beg);
        fp.write((char*)&clientObj, sizeof(clientObj));
        // cout << "\n---Wrote in mid---\n";
        fp.close();
    }
    else {
        fp.close();
        fp.open(CONNECTION, ios::app | ios::binary);
        fp.write((char*)&clientObj, sizeof(clientObj));
         // cout << "\n---Wrote at end---\n";
        fp.close();
    }
    // return true;
    
    // cout <<"\nfinished writing, now reading..\n";
    // size = get_file_size();
    
    // fp.open(CONNECTION, ios::in | ios::binary);
    // Client obj;
    
    // while((size_t)fp.tellg() < size) {
    //     fp.read((char*)&obj, sizeof(obj));
    //     print_obj(obj);
    // }
    
    // fp.close();
    // cout<<"Leaving add_client..\n";
    
    return true;
}



void remove_client(int clientFD) {
    
    // cout <<"In remove client, removing client with fd: "<<clientFD<<endl;
    // cout <<"\n\nBefore deleting..\n";
    // Client newobj;
    // fstream fp;
    // fp.open(CONNECTION, ios::in | ios::binary);
    // size_t size = get_file_size();
    // while((size_t)fp.tellg() < size) {
    //     fp.read((char*)&newobj, sizeof(newobj));
    //     print_obj(newobj);
    // }
    // fp.close();
    
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::out | ios::binary);
    
    Client readObj, nullObj;
    fp.read((char*)&readObj, sizeof(readObj));
    while(readObj.sockfd != clientFD)
        fp.read((char*)&readObj, sizeof(readObj));
    
    fp.seekp((size_t)fp.tellp() - sizeof(readObj), ios::beg);
    fp.write((char*)&nullObj, sizeof(nullObj));
    fp.close();
    
    
    
    // cout <<"\nAfter deleting..\n";
    // fp.open(CONNECTION, ios::in | ios::binary);
    // size = get_file_size();
    // while((size_t)fp.tellg() < size) {
    //     fp.read((char*)&newobj, sizeof(newobj));
    //     print_obj(newobj);
    // }
    // fp.close();
}


void send_error_msg(int clientFD, string msg) {
    
    string newMsg = red_bold + msg + regular;
    // cout<<"ready to send client: "<<newMsg<<endl;
    Message obj;
    strcpy(obj.message, newMsg.c_str());
    if(send(clientFD, obj.message, newMsg.size() + 1, MSG_DONTWAIT) < 0)
        print_error("send_error_msg to client Error");
}

void forward_msg(Client &clientObj, string msg) {

    string newMsg = magenta_bold;
    newMsg += clientObj.username;
    newMsg += ": \33[0m" + msg + "\n";
    size_t size = get_file_size(CONNECTION);
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    
    Client readObj;
    while((size_t)fp.tellg() < size) {
        fp.read((char*)&readObj, sizeof(readObj));
        if(readObj.sockfd != clientObj.sockfd &&
            !strcmp(readObj.chatroom, clientObj.chatroom)
            && readObj.state == LOGGED_IN)
            send_msg(readObj.sockfd, newMsg.c_str(), newMsg.size() + 1);
        // cout<<"\n---read---\n";
        // print_obj(readObj);
    }
    fp.close();
}