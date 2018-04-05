#include "./server.h"

int get_file_size() {
    
    fstream tempFP;
    tempFP.open(CONNECTION, ios::app | ios::binary);
    if(!tempFP)
        print_error("Error opening connection file");
    size_t size = (size_t)tempFP.tellp();
    tempFP.close();
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

void prepare_readFd(fd_set &readSet, int sockfd) {
    
    FD_ZERO(&readSet);
    FD_SET(sockfd, &readSet);
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    if(!fp)
        print_error("Prepare_readFD Error");
    
    Client obj;
    size_t size = get_file_size();
    
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

void getMaxFd(int &maxfd) {
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    Client obj;
    maxfd = -1;
    int size = get_file_size();
    
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
    
    size_t size = get_file_size();
    
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
    if(send(clientFD, (char*)&newMsg, sizeof(newMsg), 0) < 0)
    // if(send(clientFD, (char*)&newMsg, sizeof(newMsg), MSG_DONTWAIT) < 0)
        print_error("send_error_msg to client Error");
}
