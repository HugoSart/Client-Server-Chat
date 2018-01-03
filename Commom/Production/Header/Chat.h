//
// Created by hugovs on 01/01/18.
//

#ifndef SERVER_CHAT_H
#define SERVER_CHAT_H


#include <iostream>
#include "User.h"
#include <vector>
#include <map>
#include <fstream>
#include "../../Libraries/Socket/ServerSocket.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

using namespace std;
typedef struct message {

    string nickname;
    string content;

    message(string nickname, string content) {
        this->nickname = nickname;
        this->content = content;
    }

    string toString() {
        if (nickname == "/info")
            return content;
        return nickname + ": " + content;
    }

    string toStringColor() {
        if (nickname == "/info")
            return content;
        return KBLU + nickname + KNRM + ": " + content;
    }

} Message;

class Chat {
public:
    map<ServerSocket, User> users;
    vector<Message> messages;
    vector<ServerSocket*> watchList;
    string chatName;

    int port;

    Chat(int port, string chatName);

    void start();

    static void connect(string host, int port, string nickname);
    static void watch(string host, int port);

};


#endif //SERVER_CHAT_H
