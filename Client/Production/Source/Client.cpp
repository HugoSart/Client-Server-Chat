#include <iostream>
#include "../../../Commom/Libraries/Socket/ClientSocket.h"
#include "../../../Commom/Libraries/Socket/SocketException.h"
#include "../../../Commom/Production/Header/Chat.h"
#include <cstring>

#define DEFAULT_PORT 0
#define DEFAULT_HOST "localhost"
#define DEFAULT_MESSAGE ""
#define DEFAULT_NICK "anonimous"

using namespace std;

int port = DEFAULT_PORT;
char host[200] = DEFAULT_HOST;
char message[200] = DEFAULT_MESSAGE;
char nick[200] = DEFAULT_NICK;

bool viewMode = false;


int parseArgs(int argc, char *argv[]) {
    opterr = 0;
    int c;
    while ((c = getopt (argc, argv, "h:p:vn:")) != -1)
        switch (c) {
            case 'h': strcpy(host, optarg); break;
            case 'p': port = stoi(optarg);  break;
            case 'n': strcpy(nick, optarg); break;
            case 'v': viewMode = true;      break;
            default: abort();
        }
}

int main(int argc, char *argv[]) {

    parseArgs(argc, argv);

    if (!viewMode)  Chat::connect(host, port, nick);
    else            Chat::watch(host, port);

    return 0;

}
