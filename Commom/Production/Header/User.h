//
// Created by hugovs on 01/01/18.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H


#include <iostream>

using namespace std;
class User {

private:
    string nickName;

public:
    User(string nickName);
    User();

    string getNickName();
    void setNickName(string nickName);
    bool operator==(User &user);

};


#endif //SERVER_USER_H
