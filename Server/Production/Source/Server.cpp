#include <iostream>
#include <semaphore.h>
#include <sys/wait.h>
#include "../../../Commom/Production/Header/Chat.h"

using namespace std;

#define BUGOL18_PORT 12347
#define BUGOLL_PORT 12346

int main() {

    // Inicia o semárforo para os processos
    sem_t sem{};
    sem_init(&sem, 1, 1);

    // Cria um processo filho
    pid_t pid = 0;
    pid = fork();

    // Cria um pipe
    int fd[2];
    if (pipe(fd) == -1) {
        cerr << "Falha ao criar pipe." << endl;
        return 1;
    }

    Chat *chat;

    // Cria e inicia o chat de cada processo
    if (pid == 0) chat = new Chat(BUGOL18_PORT, "BUGOL +18");
    else chat = new Chat(BUGOLL_PORT, "BUGOL LIVRE");
    chat->start();

    cout << "  " << chat->chatName << " fechado." << endl;

    if (pid == 0) { // Processo filho

        sem_wait(&sem);
        sem_post(&sem);

        // Envia o número de mensagens atual no pipe
        size_t size = chat->messages.size();
        write(fd[1], &size, sizeof(size_t));
        close(fd[1]);
        close(fd[0]);

    } else { // Processo pai

        sem_wait(&sem);
        sem_destroy(&sem);

        // Recebe o número de mensagens do filho
        size_t size;

        // TODO: Programa travando aqui, se comentar funciona, mas não recebe o número de mensagem do processo filho
        read(fd[0], &size, sizeof(size_t));

        close(fd[0]);
        close(fd[1]);

        cout << "Numero total de mensagens enviadas: " + size + chat->messages.size() << endl;

    }

    delete chat;

    return 0;

}