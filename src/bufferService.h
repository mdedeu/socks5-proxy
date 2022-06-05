#ifndef BUFFER_SERVICE_H
#define BUFFER_SERVICE_H

#include "buffer.h"
#include "state/stm.h"
#include "parsing/parser.h"
#include "state/proxy_state_machine.h"

#define MAX_SIZE 2048

typedef struct bufferAndFd{
    int fd;
    int linkedFd;
    //TODO: mirar que hacer con el buffer de escritura
    buffer * wBuff;
    buffer * rBuff;
    struct state_machine * stm; 
} bufferAndFd;

int linkBuffer(int fd);

int unlinkBuffer(int fd);

int unlinkCrossedBuffers(int fd1, int fd2);

int crossLinkBuffers(int fd1, int fd2);

bufferAndFd * getBufferAndFd(int fd);

#endif