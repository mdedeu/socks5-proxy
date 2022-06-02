#include "bufferService.h"
#include <stdint.h>

bufferAndFd* buffers[MAX_SIZE] = {0};

int linkBuffer(int fd){
    if(buffers[fd] != NULL)
        return -1;

    buffers[fd] = malloc(sizeof(bufferAndFd));
    buffers[fd]->fd = fd;
    buffers[fd]->linkedFd = -1;

    buffer * wBuff = malloc(sizeof(buffer));
    buffer * rBuff = malloc(sizeof(buffer));

    uint8_t * wData = malloc(BUFFER_SIZE);
    uint8_t * rData = malloc(BUFFER_SIZE);

    buffer_init(wBuff, BUFFER_SIZE, wData);
    buffer_init(rBuff, BUFFER_SIZE, rData);

    buffers[fd]->wBuff = wBuff;
    buffers[fd]->rBuff = rBuff;

    return 0;
}

int unlinkBufferFor(int fd){

}

int crossLinkBuffers(int fd1, int fd2){
    if(buffers[fd1] != NULL || buffers[fd2] != NULL)
        return -1;

    buffers[fd1] = malloc(sizeof(bufferAndFd));
    buffers[fd1]->fd = fd1;
    buffers[fd1]->linkedFd = fd2;

    buffers[fd2] = malloc(sizeof(bufferAndFd));
    buffers[fd2]->fd = fd2;
    buffers[fd2]->linkedFd = fd1;

    buffer * wBuff = malloc(sizeof(buffer));
    buffer * rBuff = malloc(sizeof(buffer));

    uint8_t * wData = malloc(BUFFER_SIZE);
    uint8_t * rData = malloc(BUFFER_SIZE);

    buffer_init(wBuff, BUFFER_SIZE, wData);
    buffer_init(rBuff, BUFFER_SIZE, rData);

    buffers[fd1]->wBuff = wBuff;
    buffers[fd2]->rBuff = rBuff;

    buffers[fd2]->wBuff = rBuff;
    buffers[fd2]->rBuff = wBuff;

    return 0;
}

buffer * getWriteBuffer(int fd){
    if(fd < 0 || buffers[fd] == NULL)
        return NULL;
    return buffers[fd]->wBuff;
}

buffer * getReadBuffer(int fd){
    if(fd < 0 || buffers[fd] == NULL)
        return NULL;
    return buffers[fd]->rBuff;
}

int getLinkedFd(int fd){
    if(fd < 0 || buffers[fd] == NULL)
        return NULL;
    return buffers[fd]->linkedFd;
}
