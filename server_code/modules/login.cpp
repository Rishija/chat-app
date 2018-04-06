#include "../server.h"

bool credentials_ok(const char *name, const char *pass) {
    
    fstream fp;
    fp.open(DATA, ios::in | ios::binary);
    if(!fp) {
        fp.close();
        // cout<<"creating file from login credentials_ok\n";
        fp.open(DATA, ios::out | ios::binary);
        fp.close();
        return false;
    }
    
    char uName[CREDENTIAL + 1], uPass[CREDENTIAL + 1];
    while(!fp.eof()) {
        fp.read(uName, CREDENTIAL + 1);
        fp.read(uPass, CREDENTIAL + 1);
        // cout<<"read: "<<uName<<" "<<uPass<<endl;
        // cout<<"input: "<<name<<" "<<pass<<endl;
        if(!strcmp(uName, name) && !(strcmp(uPass, pass))) {
            fp.close();
            return true;
        }
    }
    fp.close();
    return false;
}

void handle_login(Client &clientObj, const string &msg) {
    
    // cout<<"In handle_login()..\n";
    // print_obj(clientObj);

    stringstream ss(msg);
    string substr;
    vector<string> tokens;

    while(!ss.eof()) {
        ss >> substr;
        // cout<<"Substr: "<<substr<<"\n";
        tokens.push_back(substr);
    }

    if(tokens.size() != 3) {
        send_msg(clientObj.sockfd, "ill_format", 11);
        return;
    }
    if(tokens[1].size() > CREDENTIAL || tokens[2].size() > CREDENTIAL) {
        send_msg(clientObj.sockfd, "ill_format", 11);
        return;
    }
    if(credentials_ok(tokens[1].c_str(), tokens[2].c_str())) {
        if(send_msg(clientObj.sockfd, "logged_in", 10)) {
            clientObj.state = LOGGED_IN;
            strcpy(clientObj.username, tokens[1].c_str());
            strcpy(clientObj.chatroom, "");
            update_client_entry(clientObj);
        }
    }
    else {
        // cout << "credentials didn't match..\n";
        send_msg(clientObj.sockfd, "mismatch", 9);
    }
}
