#include <iostream>
#include <semaphore.h>
#include <sys/wait.h>
#include "../../../Commom/Production/Header/Chat.h"

using namespace std;

#define BUGOL18_PORT 12347
#define BUGOLL_PORT 12346

int main() {

    sem_t sem{};
    sem_init(&sem, 1, 1);

    pid_t pid = 0;
    pid = fork();

    Chat *chat;

    if (pid == 0) chat = new Chat(BUGOL18_PORT, "BUGOL +18");
    else chat = new Chat(BUGOLL_PORT, "BUGOL LIVRE");

    chat->start();

    cout << "  " << chat->chatName << " fechado." << endl;

    if (pid == 0) {
        sem_wait(&sem);
        sem_post(&sem);
    } else {
        wait(NULL);
        sem_destroy(&sem);
    }

    delete chat;

    return 0;

}