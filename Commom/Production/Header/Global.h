//
// Created by hugovs on 05/01/18.
//

#ifndef SERVER_GLOBAL_H
#define SERVER_GLOBAL_H

#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define CHILD_PID 0
#define FIFO "fifo"
#define FIFO_MAX_SIZE 500

extern pid_t pid;

#endif //SERVER_GLOBAL_H
