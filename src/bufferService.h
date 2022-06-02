#ifndef bufferService.h
#define bufferService.h

#include <buffer.h>
#include <selector.h>

#define MAX_SIZE 2048

typedef struct bufferAndFd{
    int fd;
    buffer * wBuff;
    buffer * rBuff;
} bufferAndFd;

bufferAndFd * getBuffer(int fd);

int linkBufferFor(int fd, fd_interest interest);

int unlinkBufferFor(int fd, fd_interest interest);

//To perform this operation both fds shall and shall only have writing buffers linked
//The writing buffer of each will be asigned to the reading buffer of the other
int crossLinkBuffers(int fd1, int fd2);

//Neither fd should be linked to any buffer
//Both fds will be linked and crossed with each other
int linkAndCrossBuffers(int fd1, int fd2);

#endif