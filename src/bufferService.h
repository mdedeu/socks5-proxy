#ifndef BUFFER_SERVICE_H
#define BUFFER_SERVICE_H

#include "buffer.h"

#define MAX_SIZE 2048
#define BUFFER_SIZE 512

typedef struct bufferAndFd{
    int fd;
    int linkedFd;
    buffer * wBuff;
    buffer * rBuff;
} bufferAndFd;

int linkBuffer(int fd);

int unlinkBuffer(int fd);

int unlinkCrossedBuffers(int fd1, int fd2);

int crossLinkBuffers(int fd1, int fd2);

bufferAndFd * getBufferAndFd(int fd);

#endif