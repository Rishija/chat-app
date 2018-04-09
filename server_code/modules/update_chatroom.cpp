#include "../server.h"

void increase_room_count(string chatroom) {

    // cout<<"In increase_room_count..\n";
    char room[CREDENTIAL + 1];
    int count;

    fstream fp;
    fp.open(CHATROOM, ios::in | ios::out | ios::binary);
    size_t size = get_file_size(CHATROOM);

    // Read until match found or end of file
    while((size_t)fp.tellg() < size && strcmp(room, chatroom.c_str())) {
        fp.read(room, CREDENTIAL + 1);
        fp.read((char*)&count, sizeof(int));
    }

    // Entry found in the file
    if(!strcmp(room, chatroom.c_str())) {
        ++count;
        // cout<<"changing old..\n";
        fp.seekp((size_t)fp.tellg() - sizeof(int), ios::beg);
        fp.write((char*)&count, sizeof(int));
    }
    else {
        // cout<<"creating new entry.. Will write "<<chatroom<<endl;
        fp.close();
        count = 1;
        int inCount = INT_MAX;
        fp.open(CHATROOM, ios::in | ios::out | ios::binary);

        // Find free space (empty entry or end of file)
        while((size_t)fp.tellg() < size && inCount > 0) {
            fp.read(room, CREDENTIAL + 1);
            fp.read((char*)&inCount, sizeof(int));
        }

        // Free space
        if(inCount <= 0) {
            fp.seekp((size_t)fp.tellg() - CREDENTIAL - 1 - sizeof(int), ios::beg);
            fp.write(chatroom.c_str(), CREDENTIAL + 1);
            fp.write((char*)&count, sizeof(int));
        }
        // Append in end
        else {
            // fp.close();
            // fp.open(CHATROOM, ios::app | ios::binary);
            fp.write(chatroom.c_str(), CREDENTIAL + 1);
            fp.write((char*)&count, sizeof(int));
        }
    }
    fp.close();
}


void decrease_room_count(string chatroom) {

    // cout<<"In decrease_room_count..\n";
    char room[CREDENTIAL + 1];
    int count = 0;

    fstream fp;
    fp.open(CHATROOM, ios::in | ios::out | ios::binary);
    size_t size = get_file_size(CHATROOM);

    // Read until match found or end of file
    while((size_t)fp.tellg() < size && strcmp(room, chatroom.c_str())) {
        fp.read(room, CREDENTIAL + 1);
        fp.read((char*)&count, sizeof(int));
    }

    // Match found
    if(!strcmp(room, chatroom.c_str())) {
        --count;
        fp.seekp((size_t)fp.tellg() - sizeof(int), ios::beg);
        fp.write((char*)&count, sizeof(int));
    }
    // else 
    //     cout<<"Room not found\n";
    fp.close();
}
