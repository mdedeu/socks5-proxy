#include "bufferService.h"
#include <stdint.h>

bufferAndFd* buffers[MAX_SIZE] = {NULL};

int linkBuffer(int fd){
    if(fd < 0 || fd >= MAX_SIZE)
        return -1;

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

int unlinkBuffer(int fd){
    if(fd < 0 || fd >= MAX_SIZE || buffers[fd]->linkedFd != -1)
        return -1;

    if(buffers[fd] == NULL)
        return 0;
    
    free(buffers[fd]->wBuff->data);
    free(buffers[fd]->rBuff->data);

    free(buffers[fd]->wBuff);
    free(buffers[fd]->rBuff);

    free(buffers[fd]);

    buffers[fd] = NULL;

    return 0;
}

int unlinkCrossedBuffers(int fd1, int fd2){
    if(fd1 < 0 || fd2 < 0 || fd1 >= MAX_SIZE || fd2 >= MAX_SIZE)
        return -1;

    if(buffers[fd1] == NULL || buffers[fd2] == NULL)
        return -1;

    if(buffers[fd1]->linkedFd != fd2 || buffers[fd2]->linkedFd != fd1)
        return -1;
    
    free(buffers[fd1]->wBuff->data);
    free(buffers[fd1]->rBuff->data);

    free(buffers[fd1]->wBuff);
    free(buffers[fd1]->rBuff);

    free(buffers[fd1]);
    free(buffers[fd2]);

    buffers[fd1] = NULL;
    buffers[fd2] = NULL;

    return 0;
}

int crossLinkBuffers(int fd1, int fd2){
    if(fd1 < 0 || fd2 < 0 || fd1 >= MAX_SIZE || fd2 >= MAX_SIZE)
        return -1;

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

bufferAndFd * getBufferAndFd(int fd){
    if(fd < 0 || fd >= MAX_SIZE || buffers[fd] == NULL)
        return NULL;
    return buffers[fd];
}
