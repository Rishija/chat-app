#include "../server.h"

void handle_other_commands(Client &clientObj, string msg) {

	// cout<<"In handle_other_commands()..\n";
	if(msg == "\\logout") {
		handle_logout(clientObj);
		return;
	}

    stringstream ss(msg);
    string substr;
    vector<string> tokens;

    // Break command in tokens
    while(!ss.eof()) {
        ss >> substr;
        tokens.push_back(substr);
    }

    // Because user state is LOGGED_OUT
    if(tokens[0] == "\\signup" || tokens[0] == "\\login") {
    	cout<<"Sending error msg\n";
    	send_error_msg(clientObj.sockFD, "\rAlready logged in\n");
    	return;
    }

    if(tokens[0] == "\\chatrooms" && tokens.size() == 1) {
    	send_chatrooms(clientObj.sockFD);
    	return;
    }

    if(tokens[0] == "\\join" && tokens.size() == 2 && tokens[1].size() <= CREDENTIAL) {
        // Send leaving to client's chatroom
        forward_msg(clientObj, "LEFT", true);
        // Decrease count of chatroom client was member of
        decrease_room_count(clientObj.chatroom);

        // Update new chatroom
    	strcpy(clientObj.chatroom, tokens[1].c_str());
        // Increase count for chatroom client joined
    	increase_room_count(tokens[1]);
        // Update chatroom for client
    	update_client_entry(clientObj);
        // Send arrival of client to other members of chatroom
    	forward_msg(clientObj, "JOINED", true);
    	return;
    }

    // Personal message
    if(tokens[0] == "\\send" && tokens.size() > 2 && tokens[1].size() <= CREDENTIAL) {

    	string temp = magenta_bold;
    	temp += clientObj.username;
    	int i;
        // Find start of message to be send
    	for(i = 6; ; ++i) {
    		if(msg[i] == ' ')
    			break;
    	}
        // Format message
    	temp += " (P) : \33[0m" + msg.substr(i + 1) + "\n";
    	send_msg_by_name(clientObj.sockFD, tokens[1], temp);
    	return;
    }
    send_error_msg(clientObj.sockFD, "\rInvalid action\n");
}
