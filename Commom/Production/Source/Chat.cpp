//message
// Created by hugovs on 01/01/18.
//

#include <utility>
#include <algorithm>

#include <pthread.h>
#include "../Header/Chat.h"
#include "../../Libraries/Socket/ServerSocket.h"
#include "../../Libraries/Socket/SocketException.h"
#include "../../Libraries/Socket/ClientSocket.h"

typedef struct targ {

    Chat *chat;
    ServerSocket *serverSocket;

    targ(Chat *chat, ServerSocket *serverSocket) {
        this->chat = chat;
        this->serverSocket = serverSocket;
    }

} TArg;

void *chat_thread(void *arg) {

    auto tArg = (TArg *) arg;
    auto chat = tArg->chat;
    auto serverSocket = tArg->serverSocket;

    User user = chat->users[*serverSocket];

    cout << "  " << chat->chatName << " -> " << KGRN << user.getNickName() << " entrou no chat!" << KNRM << endl;
    chat->messages.push_back(Message("/info", ("  " KGRN) + user.getNickName() + (" entrou no chat!" KNRM)));

    ofstream history("log.txt", ofstream::app);

    try {

        while (true) {

            string data;
            *serverSocket >> data;

            chat->messages.push_back(Message(user.getNickName(), data));
            cout << "  " << chat->chatName << " -> " << chat->messages.at(chat->messages.size() - 1).toStringColor() << KNRM << endl;
            history << chat->chatName << " -> " << chat->messages.at(chat->messages.size() - 1).toString() << KNRM << endl;
            *serverSocket << "Mensagem recebida.";

        }

    } catch (SocketException &) {}

    cout << "  " << chat->chatName << (" -> " KRED) << user.getNickName() + " saiu do chat." << KNRM << endl;
    chat->messages.push_back(Message("/info", ("  " KRED) + user.getNickName() + (" saiu do chat." KNRM)));
    delete serverSocket;

    pthread_exit(nullptr);

}

void *watch_thread(void *arg) {

    auto tArg = (TArg *) arg;
    auto chat = tArg->chat;
    auto serverSocket = tArg->serverSocket;

    try {
        unsigned long lastSize = chat->messages.size();
        while (true) {
            unsigned long size = chat->messages.size();
            if (lastSize != size) {
                for (unsigned long i = lastSize - 1; i < size - 1; i++) {
                    string msg;
                    *serverSocket >> msg;
                    try {
                        *serverSocket << chat->messages.at(i).toStringColor();
                    } catch (std::out_of_range &e) {
                        cout << e.what() << endl;
                    }
                }
                lastSize = size;
            }
        }
    } catch (SocketException &) {}

    delete serverSocket;

    pthread_exit(nullptr);

}

void *console_thread(void *arg) {

    auto *tArg = (TArg *)arg;
    Chat *chat = tArg->chat;

    while (true) {

        string carg;
        cin >> carg;

        if (carg == "/exit") {
            Chat::connect("localhost", chat->port, "/exit");
            break;
        }

    }

    pthread_exit(nullptr);

}

Chat::Chat(int port, string chatName) {
    this->port = port;
    this->chatName = std::move(chatName);
}

string Chat::messagesToString() {

    string ret;
    for (Message &m : this->messages) {
        ret.append(m.nickname);
        ret.append(": ");
        ret.append(m.content);
        ret.append("\n");
    }

    return ret;

}

void Chat::start() {

    try {

        ServerSocket server(port);

        cout << "Servidor do chat criado com sucesso!" << endl;
        cout << "=============================== " << chatName << " ===============================" << endl;
        cout << endl;

        vector<pthread_t> tids;

        tids.push_back(pthread_t());
        pthread_create(&tids.back(), nullptr, console_thread, (void *) (new TArg(this, nullptr)));

        while (true) {

            auto *newSocket = new ServerSocket();
            server.accept(*newSocket);
            string nickname;
            *newSocket >> nickname;
            *newSocket << chatName;

            tids.push_back(pthread_t());

            if (nickname == "/exit") {
                break;
            } else if (nickname == "/watch") {
                watchList.push_back(newSocket);
                pthread_create(&tids.back(), nullptr, watch_thread, (void *) (new TArg(this, newSocket)));
            } else {
                users[*newSocket] = User(nickname);
                pthread_create(&tids.back(), nullptr, chat_thread, (void *) (new TArg(this, newSocket)));
            }

        }

    } catch (SocketException &e) {
        cout << "Exception: " + e.description() << endl;
    }

}

void Chat::connect(string host, int port, string nickname) {

    try {

        ClientSocket clientSocket(std::move(host), port);
        string reply;
        try {

            clientSocket << nickname;
            clientSocket >> reply;

            if (nickname != "/exit") cout << "Você entrou no chat " << reply << " como " << nickname << endl;

            while (true) {

                char message[200];
                cout << nickname << ": ";
                cin.getline(message, sizeof(message));

                if (message == "/exit")
                    break;

                clientSocket << message;
                clientSocket >> reply;

            }
        } catch (SocketException &) {}
        cout << reply;
    }
    catch (SocketException &e) {
        cout << "Falha ao estabelecer conexão." << endl;
    }

}

void Chat::watch(string host, int port) {

    try {

        ClientSocket clientSocket(std::move(host), port);
        string reply;

        clientSocket << "/watch";
        clientSocket >> reply;

        cout << "=============================== " << reply << " ===============================" << endl;

        while (true) {
            cout << endl;
            clientSocket << "/watch";
            clientSocket >> reply;
            cout << "  " << reply;
        }

    }
    catch (SocketException &e) {
        cout << "Falha ao estabelecer conexão." << e.description() << endl;
    }

}
