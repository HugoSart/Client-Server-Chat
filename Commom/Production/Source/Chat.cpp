//message
// Created by hugovs on 01/01/18.
//

#include <utility>
#include <algorithm>

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

    auto tArg = (TArg *)arg;
    auto chat = tArg->chat;
    auto serverSocket = tArg->serverSocket;

    User user = chat->users[*serverSocket];

    try {

        while (true) {

            string data;
            *serverSocket >> data;

            chat->messages.push_back(Message(user.getNickName(), data));
            cout << "  " << chat->messages.at(chat->messages.size() - 1).toString() << endl;
            *serverSocket << "Mensagem recebida.";

        }

    } catch (SocketException &) {}

    cout << "  " << user.getNickName() + " saiu do chat." << endl;
    delete serverSocket;

    pthread_exit(nullptr);

}

Chat::Chat(int port) {
    this->port = port;
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
        cout << "=============================== CHAT ===============================" << endl;
        cout << endl;

        vector<pthread_t> tids;

        while (true) {

            auto *newSocket = new ServerSocket();
            server.accept(*newSocket);
            string nickname;
            *newSocket >> nickname;
            *newSocket << "Login efetuado.";

            users[*newSocket] = User(nickname);
            cout << "  " << users[*newSocket].getNickName() << " entrou no chat!" << endl;
            tids.push_back(pthread_t());
            pthread_create(&tids.back(), nullptr, chat_thread, (void *)(new TArg(this, newSocket)));

        }

    } catch (SocketException &e) {
        cout << "Exception: " + e.description() << endl;
    }

}

void Chat::connect(string host, int port, string nickname) {

    try {

        ClientSocket clientSocket(std::move(host), port);
        cout << "Você entrou no chat como " << nickname << endl;
        string reply;
        try {

            clientSocket << nickname;
            clientSocket >> reply;

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
