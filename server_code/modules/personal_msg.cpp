#include "../server.h"

void send_msg_by_name(int sockFD, string name, string msg) {

    size_t size = get_file_size(CONNECTION);

    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);

    char inputName[CREDENTIAL + 1];
    strcpy(inputName, name.c_str());

    Client readObj;
    while((size_t)fp.tellg() < size && strcmp(inputName, readObj.username)) {
        fp.read((char*)&readObj, sizeof(readObj));
        // print_obj(readObj);
    }

    // Send message only if user is logged in
    if(!strcmp(inputName, readObj.username) && readObj.state == LOGGED_IN) {

    	if(!send_msg(readObj.sockFD, msg.c_str(), msg.size() + 1))
    		send_error_msg(sockFD, "\r\nCouldn't send personal message\n");
    }
    else
        // No person with given name is in connections list
    	send_error_msg(sockFD, "\rFriend not online\n");
    fp.close();
}
