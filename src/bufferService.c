#include "bufferService.h"

bufferAndFd* buffers[MAX_SIZE];

bufferAndFd * getBuffer(int fd){
    if(fd < 0)
        return NULL;
    for(int i = 0; i < MAX_SIZE; i++){
        if(buffers[i] != NULL && buffers[i]->fd == fd){
            return buffers[i];
        }
    }
    return NULL;
}

int linkBufferFor(int fd, fd_interest interest){

}

int unlinkBufferFor(int fd, fd_interest interest);

//To perform this operation both fds shall and shall only have writing buffers linked
//The writing buffer of each will be asigned to the reading buffer of the other
int crossLinkBuffer(int fd1, int fd2);

//Neither fd should be linked to any buffer
//Both fds will be linked and crossed with each other
int linkAndCrossBuffer(int fd1, int fd2){
    if(isLinkedFor(fd1) || isLinkedFor(fd2))
    if(linkBufferFor(fd1, OP_WRITE) == -1){
        return -1;
    }

    if(linkBufferFor(fd2, OP_WRITE) == -1){
        unlinkBufferFor(fd2, )
    }
}

static int getFreePosition(){
    for(int i = 0; i < MAX_SIZE; i++){
        if(buffers[i] == NULL){
            return i;
        }
    }
    return -1;
}

//To check if fd is linked (for anything) use interest = OP_NOOP
static int isLinkedFor(int fd, fd_interest interest){
    if(fd < 0)
        return NULL;
    for(int i = 0; i < MAX_SIZE; i++){
        if(buffers[i] != NULL && buffers[i]->fd == fd){
            return buffers[i];
        }
    }
    return NULL;
}