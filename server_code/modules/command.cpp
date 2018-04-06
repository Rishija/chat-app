#include "../server.h"

void send_chatrooms(int sockfd) {

	cout<<"In send_chat_rooms()..\n";

	size_t size = get_file_size(CHATROOM);
	string list = "\r********\n";
	char room[CREDENTIAL + 1], empty[CREDENTIAL + 1];
	int count, zero = 0;

	fstream fp;
	fp.open(CHATROOM, ios::in | ios::out | ios::binary);
	while((size_t)fp.tellg() < size) {
		fp.read(room, CREDENTIAL + 1);
		fp.read((char*)&count, sizeof(int));

		// Delete entry
		if(count < 1) {
			fp.seekp((size_t)fp.tellg() - CREDENTIAL - 1 - sizeof(int), ios::beg);
			fp.write(empty, CREDENTIAL + 1);
			fp.read((char*)&zero, sizeof(int));
		}
		if(count > 0) {
			list += "- ";
			list += room;
			list += "\n";
		}
	}
	fp.close();
	list += "********\n";
	send_msg(sockfd, list.c_str(), list.size() + 1);
}

void increase_room_count(string chatroom) {

	cout<<"In increase_room_count..\n";
	char room[CREDENTIAL + 1];
	int count;

	fstream fp;
	fp.open(CHATROOM, ios::in | ios::out | ios::binary);
	size_t size = get_file_size(CHATROOM);
	// cout<<"size of file: "<<size<<endl;

	while((size_t)fp.tellg() < size && strcmp(room, chatroom.c_str())) {
		fp.read(room, CREDENTIAL + 1);
		fp.read((char*)&count, sizeof(int));
	}

	if(!strcmp(room, chatroom.c_str())) {
		++count;
		cout<<"changing old..\n";
		fp.seekp((size_t)fp.tellg() - sizeof(int), ios::beg);
		fp.write((char*)&count, sizeof(int));
	}
	else {
		cout<<"creating new entry.. Will write"<<chatroom<<endl;
		fp.close();
		count = 1;
		fp.open(CHATROOM, ios::app | ios::binary);
		fp.write(chatroom.c_str(), CREDENTIAL + 1);
		fp.write((char*)&count, sizeof(int));
	}
	fp.close();



	// cout<<"Finished writing, now reading..\n";
	// size = get_file_size(CHATROOM);
	// fp.open(CHATROOM, ios::in | ios::binary);
	// while((size_t)fp.tellg() < size) {
	// 	fp.read(room, CREDENTIAL + 1);
	// 	fp.read((char*)&count, sizeof(int));
	// 	cout << "chatroom: "<<room<<" "<<count<<endl;
	// }
	// fp.close();
}

void decrease_room_count(string chatroom) {

	cout<<"In decrease_room_count..\n";
	char room[CREDENTIAL + 1];
	int count = 0;

	fstream fp;
	fp.open(CHATROOM, ios::in | ios::out | ios::binary);
	size_t size = get_file_size(CHATROOM);

	while((size_t)fp.tellg() < size && strcmp(room, chatroom.c_str())) {
		fp.read(room, CREDENTIAL + 1);
		fp.read((char*)&count, sizeof(int));
	}

	if(!strcmp(room, chatroom.c_str())) {
		--count;
		fp.seekp((size_t)fp.tellg() - sizeof(int), ios::beg);
		fp.write((char*)&count, sizeof(int));
	}
	else {
		cout<<"Not found\n";
		// fp.write(chatroom.c_str(), CREDENTIAL + 1);
		// fp.write((char*)&count, sizeof(int));
	}
	fp.close();
}

void send_msg_by_name(int sockfd, string name, string msg) {

    size_t size = get_file_size(CONNECTION);
    
    fstream fp;
    fp.open(CONNECTION, ios::in | ios::binary);

    char inputName[CREDENTIAL + 1];
    strcpy(inputName, name.c_str());
    
    Client readObj;
    while((size_t)fp.tellg() < size && strcmp(inputName, readObj.username)) {
        fp.read((char*)&readObj, sizeof(readObj));
        // cout<<"\n---read---\n";
        // print_obj(readObj);
    }
    
    if(!strcmp(inputName, readObj.username) && readObj.state == LOGGED_IN) {

    	if(!send_msg(readObj.sockfd, msg.c_str(), msg.size() + 1))
    		send_error_msg(sockfd, "\r\nCouldn't send personal message\n");
    }
    else
    	send_error_msg(sockfd, "\rFriend not online\n");
    fp.close();
}

void handle_command(Client &clientObj, string msg) {

	cout<<"In handle_command()..\n";
	if(msg == "\\logout") {
		handle_logout(clientObj);
		return;
	}

    stringstream ss(msg);
    string substr;
    vector<string> tokens;

    while(!ss.eof()) {
        ss >> substr;
        tokens.push_back(substr);
    }


    if(tokens[0] == "\\signup" || tokens[0] == "\\login") {
    	cout<<"Sending error msg\n";
    	send_error_msg(clientObj.sockfd, "\rAlready logged in\n");
    	return;
    }

    if(tokens[0] == "\\chatroom" && tokens.size() == 1) {
    	send_chatrooms(clientObj.sockfd);
    	return;
    }

    if(tokens[0] == "\\join" && tokens.size() == 2 && tokens[1].size() <= CREDENTIAL) {
    	strcpy(clientObj.chatroom, tokens[1].c_str());
    	increase_room_count(tokens[1]);
    	update_client_entry(clientObj);
    	forward_msg(clientObj, "Hi there! I just joined");
    	return;
    }

    if(tokens[0] == "\\send" && tokens.size() > 2 && tokens[1].size() <= CREDENTIAL) {
    	string temp = magenta_bold;
    	temp += clientObj.username;
    	int i;
    	for(i = 6; ; ++i) {
    		if(msg[i] == ' ')
    			break;
    	}
    	temp += ": \33[0m" + msg.substr(i + 1) + "\n";
    	send_msg_by_name(clientObj.sockfd, tokens[1], temp);
    	return;
    }

    send_error_msg(clientObj.sockfd, "\rInvalid action\n");
}
