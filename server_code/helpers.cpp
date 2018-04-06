#include "./server.h"

void prepare_readFd(fd_set &readSet, int sockFD, int recentConn) {

    FD_ZERO(&readSet);
    FD_SET(sockFD, &readSet);
    // Add recently added connection to read fd set
    FD_SET(recentConn, &readSet);

    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    if(!fp)
        print_error("Prepare_readFD Error");

    Client obj;
    size_t size = get_file_size(CONNECTION);

    // Add current connections to read fd set
    while((size_t)fp.tellg() < size) {
        fp.read((char*)&obj, sizeof(obj));
        if(obj.sockFD != -1)
            FD_SET(obj.sockFD, &readSet);
    }

    fp.close();
}


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


void getMaxFd(int &maxfd, int recentConn) {

    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);
    Client obj;
    maxfd = recentConn;
    int size = get_file_size(CONNECTION);
    
    while((int)fp.tellg() < size) {
        fp.read((char*)&obj, sizeof(obj));
        maxfd = max(maxfd, obj.sockFD);
    }

    fp.close();
}


void create_connection_file(int listenFD, sockaddr_in &servAddr) {

    fstream fp;
    fp.open(CONNECTION, ios::out | ios::binary);
    if(!fp)
        print_error("Create connection file Error");

    // Add server listen fd to connections file
    Client serverListenFD(listenFD, inet_ntoa(servAddr.sin_addr), ntohs(servAddr.sin_port), "server");
    fp.write((char*)&serverListenFD, sizeof(serverListenFD));
    // cout<<"\nCREATED\n";
    fp.close();
}


bool add_client(Client &clientObj) {

    size_t size = get_file_size(CONNECTION);

    fstream fp;
    fp.open(CONNECTION, ios::in | ios::out | ios::binary);
    if(!fp)
        return false;

    Client readObj(1);
    while((size_t)fp.tellg() < size && readObj.sockFD > 0) {
        fp.read((char*)&readObj, sizeof(readObj));
    }

    // Found empty entry, write over this
    if(readObj.sockFD < 0) {
        // Seek back to empty entry
        fp.seekp((size_t)fp.tellg() - sizeof(readObj), ios::beg);
        fp.write((char*)&clientObj, sizeof(clientObj));
        fp.close();
    }
    // Append in the file
    else {
        fp.close();
        fp.open(CONNECTION, ios::app | ios::binary);
        fp.write((char*)&clientObj, sizeof(clientObj));
        fp.close();
    }
    return true;
}


void remove_client(int clientFD) {

    fstream fp;
    fp.open(CONNECTION, ios::in | ios::out | ios::binary);

    Client readObj, nullObj;
    fp.read((char*)&readObj, sizeof(readObj));
    while(readObj.sockFD != clientFD)
        fp.read((char*)&readObj, sizeof(readObj));

    // Seek back to matched client postion
    fp.seekp((size_t)fp.tellp() - sizeof(readObj), ios::beg);
    // Write empty entry
    fp.write((char*)&nullObj, sizeof(nullObj));
    fp.close();
}


void update_client_entry(Client &clientObj) {

    size_t size = get_file_size(CONNECTION);

    fstream fp;
    fp.open(CONNECTION, ios::in | ios::out | ios::binary);

    Client readObj;
    while((size_t)fp.tellg() < size && readObj.sockFD != clientObj.sockFD) {
        fp.read((char*)&readObj, sizeof(readObj));
        // print_obj(readObj);
    }

    // Seek back to required postion and write new object
    if(readObj.sockFD == clientObj.sockFD) {
        fp.seekp((size_t)fp.tellg() - sizeof(readObj), ios::beg);
        fp.write((char*)&clientObj, sizeof(clientObj));
    }
    // else
    //     cout << "Client not found";
    fp.close();
}


void send_error_msg(int clientFD, string msg) {

    // Formatting
    string newMsg = red_bold + msg + regular;
    char msgToSend[MAX];
    strcpy(msgToSend, newMsg.c_str());

    if(send(clientFD, msgToSend, newMsg.size() + 1, MSG_DONTWAIT) < 0)
        print_error("send_error_msg to client Error");
}


int get_file_size(string fileName) {

    fstream tempFP;
    tempFP.open(fileName, ios::app | ios::binary);
    if(!tempFP)
        print_error("Error opening connection file");
    size_t size = (size_t)tempFP.tellp();
    tempFP.close();
    return size;
}


void print_obj(Client &obj) {

    cout << "Sock: " << obj.sockFD
    << "\n ip: " << obj.ip
    << "\n port: " << ntohs(obj.port)
    << "\n username: " << obj.username
    << "\n chatroom: " << obj.chatroom << endl;
}
