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

void tcpConnectionHandler(struct selector_key *key);
void readSocketHandler(struct selector_key *key);
void writeSocketHandler(struct selector_key *key);

struct buffer_and_fd{
    int fd;
    buffer * wBuff;
    buffer * rBuff;
};

//Handlers estandares para sockets activos y pasivos
fd_handler passiveFdHandler = {
    .handle_read = &tcpConnectionHandler,
    .handle_write = NULL,
    .handle_block = NULL,
    .handle_close = NULL
};

fd_handler activeFdHandler = {
    .handle_read = &readSocketHandler,
    .handle_write = &writeSocketHandler,
    .handle_block = NULL,
    .handle_close = NULL
};


void readSocketHandler(struct selector_key *key){
    struct buffer_and_fd * b = (struct buffer_and_fd*)key->data- ;
    char temp[1024];
    int nread;
    if( buffer_can_write() ){
        nread = recv(b->fd,temp,1024,MSG_DONTWAIT);
        buffer_write(nread,temp);
        selector_register(key->s, newSocket->fd, &readSocketHandler,OP_READ,&newSocket);
    }
}


void writeSocketHandler(struct selector_key * key ){

}



//El cliente se conecta conmigo a traves del socket pasivo tcp (masterSocket)
//
//  (masterSock)
// Cli -----> Proxy
//
//Abro un socket activo tcp (cliSock) para comunicarme con este
//y un socket activo tcp (serSock) para comunicarme con el server destino
//
//   (cliSock)   (serSock)
// Cli <---> Proxy <---> Server

//TODO: ver como manejar errores de las syscalls!!!
void tcpConnectionHandler(struct selector_key *key){

    //Abro socket para comunicarme con el cliente
    struct sockaddr_in cliSockAddr;
    socklen_t cliSockAddrSize= sizeof(cliSockAddr);

    //TODO: asegurar que el accept no bloquee
    int cliSockFd = accept(key->fd, &cliSockAddr, &cliSockAddrSize);

    //TODO: mirar que hacer con el campo data (el ultimo)
    selector_register(key->s, cliSockFd, &activeFdHandler, OP_READ | OP_WRITE, NULL);


    //Abro socket para comunicarme con el server
    int serSockFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serSockAddr = {.sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
        .sin_port = htons(OTHERPORT)};

    connect(serSockFd, &serSockAddr, sizeof(serSockAddr));

    //TODO: mirar que hacer con el campo data (el ultimo)
    selector_register(key->s, serSockFd, &activeFdHandler, OP_READ | OP_WRITE, NULL);
}


int main(){
    int masterSocket[2];
    int masterSocketSize=0;

    if( (masterSocket[masterSocketSize] = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        goto error;
    } else {
        //set master socket to allow multiple connections , this is just a good habit, it will work without this
        if( setsockopt(masterSocket[masterSocketSize], SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
        {
            goto error;
        }

        //type of socket created
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );

        // bind the socket to localhost port 8888
        if (bind(masterSocket[masterSocketSize], (struct sockaddr *)&address, sizeof(address))<0)
        {
            close(masterSocket[masterSocketSize]);
            goto error;
        }
        else {
            if (listen(masterSocket[0], MAX_PENDING_CONNECTIONS) < 0)
            {
                close(masterSocket[masterSocketSize]);
                goto error;
            } else {
                masterSocketSize++;
            }
        }
    }

    buffer * buffer1 ;
    char bufferArray[1024];
    buffer_init(&buffer1, 1024, &bufferArray);



    size_t  initial_elements = 1 ;
    fd_selector fdSelector = selector_new(initial_elements);

    struct buffer_and_fd newClient;

    selector_register(fdSelector, masterSocket[0], &passiveFdHandler, OP_READ, &newClient);
    selector_select(fdSelector);

    error:
        return -1;
}