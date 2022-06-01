#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include "selector.h"
#include "stm.h"
#include "buffer.h"
#define MAX_PENDING_CONNECTIONS   3    // un valor bajo, para realizar pruebas
#define PORT 8888
#define OTHERPORT 9090

struct buffer_and_fd{
    int fd;
    buffer * buffer1;
};



void reading_from_client_socket      (struct selector_key *key){
    struct buffer_and_fd * b = (struct buffer_and_fd*)key->data- ;
    char temp[1024];
    int nread;
    if( buffer_can_write() ){
        nread = recv(b->fd,temp,1024,MSG_DONTWAIT);
        buffer_write(nread,temp);
        selector_register(key->s, newSocket->fd, reading_from_client_socket(),OP_READ,&newSocket);
    }
}

void writing_on_dest_socket(struct selector_key * key ){

}


void handle_read_connect      (struct selector_key *key){
    sockaddr otherData;
    socklen_t  otherDataSize = sizeof (otherData);
    struct buffer_and_fd * newSocket = (struct buffer_and_fd * )key->data;
    * newSocket = accept(key->fd,&otherData,&otherDataSize);


    struct sockaddr_in connectionData = {.sin_family=AF_INET,.sin_addr.s_addr=inet_addr("127.0.0.1"),.sin_port=htons(OTHERPORT)};
    int serverSocketFd = connect();

    selector_register(key->s, newSocket->fd, reading_from_client_socket(),OP_READ,&newSocket);
    selector_register(key->,serverSocketFd,writing_on_dest_socket,OP_WRITE,&newSocket);

}


int main(){
    int master_socket[2];
    int master_socket_size=0;

    if( (master_socket[master_socket_size] = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        goto error;
    } else {
        //set master socket to allow multiple connections , this is just a good habit, it will work without this
        if( setsockopt(master_socket[master_socket_size], SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
        {
            goto errror;
        }

        //type of socket created
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );

        // bind the socket to localhost port 8888
        if (bind(master_socket[master_socket_size], (struct sockaddr *)&address, sizeof(address))<0)
        {
            close(master_socket[master_socket_size]);
            goto error;
        }
        else {
            if (listen(master_socket[0], MAX_PENDING_CONNECTIONS) < 0)
            {
                close(master_socket[master_socket_size]);
                goto error;
            } else {
                master_socket_size++;
            }
        }
    }

    buffer * buffer1 ;
    char bufferArray[1024];
    buffer_init(&buffer1,1024,&bufferArray);



    size_t  initial_elements = 1 ;
    fd_selector fdSelector = selector_new(initial_elements);
    struct buffer_and_fd newClient;
    fd_handler  connectHandler ={.handle_read=handle_read_connect};
    selector_register(fdSelector,master_socket[0],    connectHandler,    OP_READ , &newClient);
    selector_select(fdSelector);

    error:
        return -1;
}