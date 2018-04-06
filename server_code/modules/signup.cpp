#include "../server.h"

bool valid_username(const char *name) {
    
    fstream fp;
    fp.open(DATA, ios::in | ios::out | ios::binary);
    if(!fp) {
        fp.close();
        fp.open(DATA, ios::out | ios::binary);
        fp.close();
        return true;
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
    
    fp.write(name, CREDENTIAL + 1);
    fp.write(pass, CREDENTIAL + 1);
    fp.close();

    cout<<"Added to data file. Now reading data file..\n";
    fp.open(DATA, ios::in | ios::binary);

    char uName[CREDENTIAL + 1], uPass[CREDENTIAL + 1];
    while(!fp.eof()) {
        fp.read(uName, CREDENTIAL + 1);
        fp.read(uPass, CREDENTIAL + 1);
        cout<<"read: "<<uName<<" "<<uPass<<endl;
    }
    fp.close();
    return true;
}

void handle_signup(Client &clientObj, const string &msg) {
    
    cout<<"In handle_signup()..\n";
    print_obj(clientObj);

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
    if(valid_username(tokens[1].c_str())) {
        if(add_to_data(tokens[1].c_str(), tokens[2].c_str()) &&
           send_msg(clientObj.sockfd, "signed_up", 10)) {
            
            cout<<"signed up..\n";
            clientObj.state = LOGGED_IN;
            strcpy(clientObj.username, tokens[1].c_str());
            update_client_entry(clientObj);
        }
    }
    else {
        cout<<"username taken..\n";
        send_msg(clientObj.sockfd, "username_taken", 15);
    }
}
