#include <iostream>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../../../Commom/Production/Header/Chat.h"
#include "../../../Commom/Production/Header/Global.h"

using namespace std;

#define BUGOLL_PORT 12346
#define BUGOLS_PORT 12347

#define SNAME "/mysem"

int main(int argc, char *argv[]) {

    int port1 = BUGOLS_PORT, port2 = BUGOLL_PORT;

    if (argv[2] != nullptr)
        port1 = stoi(argv[2]);
    if (argv[3] != nullptr)
        port2 = stoi(argv[3]);

    // Cria um pipe
    int fd[2];
    pipe(fd);

    // Cria um processo filho
    pid = CHILD_PID;
    pid = fork();

    // Cria FIFO
    mkfifo(FIFO, 0666);

    Chat *chat;
    sem_t *sem;

    // Cria e instancia o chat de cada processo
    if (pid == CHILD_PID) {
        chat = new Chat(port1, "BUGOL ESPORTES");
        sem = sem_open(SNAME, O_CREAT, 0644, 1);
    } else {
        chat = new Chat(port2, "BUGOL LIVRE");
        sem = sem_open(SNAME, 0);
    }

    chat->start();

    cout << "  " << chat->chatName << " fechado." << endl;

    size_t size = chat->messages.size();

    if (pid == CHILD_PID) { // Processo filho

        close(fd[0]);

        write(fd[1], &size, sizeof(size));
        close(fd[1]);

        sem_post(sem);

    } else { // Processo pai

        sem_wait(sem);
        close(fd[1]);

        size_t childSize;

        read(fd[0], &childSize, sizeof(childSize));
        close(fd[0]);

        cout << "Numero total de eventos ocorridos: " << (size + childSize) << endl;

        sem_destroy(sem);
        wait(nullptr);

    }

    delete chat;

    return 0;

}