#include <iostream>
#include "../../../Commom/Production/Header/Chat.h"

using namespace std;

#define DEFAULT_PORT 3000

int main() {

    Chat chat(DEFAULT_PORT);
    chat.start();

    return 0;

}