#include "../server.h"

void send_chatrooms(int sockFD) {

	size_t size = get_file_size(CHATROOM);
	// Formatting
	string list = "\r********\n";
	char room[CREDENTIAL + 1], empty[CREDENTIAL + 1];
	int count, zero = 0;

	fstream fp;
	fp.open(CHATROOM, ios::in | ios::out | ios::binary);
	while((size_t)fp.tellg() < size) {
		fp.read(room, CREDENTIAL + 1);
		fp.read((char*)&count, sizeof(int));

		// Add only chatromms with atleast 1 member
		if(count > 0) {
			list += "- ";
			list += room;
			list += "\n";
		}
	}
	fp.close();
	list += "********\n";
	send_msg(sockFD, list.c_str(), list.size() + 1);
}
