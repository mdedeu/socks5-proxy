#ifndef bufferService.h
#define bufferService.h

#include <buffer.h>
#include <selector.h>

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

int crossLinkBuffers(int fd1, int fd2);

buffer * getWriteBuffer();

buffer * getReadBuffer();

int getFd();

int getLinkedFd();

#endif