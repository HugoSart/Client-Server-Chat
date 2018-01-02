//
// Created by hugovs on 01/01/18.
//

#ifndef SERVER_CHAT_H
#define SERVER_CHAT_H


#include <iostream>
#include "User.h"
#include <vector>
#include <map>
#include "../../Libraries/Socket/ServerSocket.h"

using namespace std;
typedef struct message {

    string nickname;
    string content;

    message(string nickname, string content) {
        this->nickname = nickname;
        this->content = content;
    }

    string toString() {
        return nickname + ": " + content;
    }

} Message;

class Chat {
public:
    map<ServerSocket, User> users;
    vector<Message> messages;

    int port;

    Chat(int port);

    string messagesToString();
    void start();

    static void connect(string host, int port, string nickname);

};


#endif //SERVER_CHAT_H
