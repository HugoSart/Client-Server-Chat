//
// Created by hugovs on 01/01/18.
//

#include <utility>
#include "../Header/User.h"

User::User(string nickname) {
    this->setNickName(std::move(nickname));
}

User::User() {}

string User::getNickName() {
    return this->nickName;
}

void User::setNickName(string nickName) {
    this->nickName = std::move(nickName);
}

bool User::operator==(User &user) {
    return this->getNickName() == user.getNickName();
}
