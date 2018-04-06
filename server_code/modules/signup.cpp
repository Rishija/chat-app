#include "../server.h"

bool valid_username(const char *name) {
    
    fstream fp;
    fp.open(DATA, ios::in | ios::binary);
    if(!fp) {
        print_error("Data file Error");
        return false;
    }
    
    char uName[CREDENTIAL + 1], uPass[CREDENTIAL + 1];
    while(!fp.eof()) {
        fp.read(uName, CREDENTIAL + 1);
        fp.read(uPass, CREDENTIAL + 1);
        if(!strcmp(uName, name)) {
            fp.close();
            return false;
        }
    }
    fp.close();
    return true;
}

bool add_to_data(const char *name, const char *pass) {
    
    fstream fp;
    fp.open(DATA, ios::app | ios::binary);
    if(!fp) {
        print_error("Data file Error");
        return false;
    }
    
    fp.write(name, sizeof(name));
    fp.write(pass, sizeof(pass));
    fp.close();
    return true;
}

void handle_signup(Client &clientObj, const string &msg) {
    
    stringstream ss(msg);
    string substr;
    ss >> substr;
    vector<string> tokens;
    while(substr != "") {
        tokens.push_back(substr);
        ss >> substr;
    }
    
    if(tokens.size() != 3) {
        send_msg(clientObj.sockfd, "ill_format", 11);
        return;
    }
    if(tokens[1].size() > CREDENTIAL || tokens[2].size() > CREDENTIAL) {
        send_msg(clientObj.sockfd, "ill_format", 11);
        return;
    }
    if(valid_username(tokens[1].c_str())) {
        if(add_to_data(tokens[1].c_str(), tokens[2].c_str()) &&
           send_msg(clientObj.sockfd, "signed_up", 10)) {
            
            clientObj.state = LOGGED_IN;
            strcpy(clientObj.username, tokens[1].c_str());
            update_client_entry(clientObj);
        }
    }
    else
        send_msg(clientObj.sockfd, "username_taken", 15);
}
