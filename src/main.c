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
#define OTHER_PORT 9090
#define TRUE 1
#define RW_AMOUNT 30

void tcpConnectionHandler(struct selector_key *key);
void readSocketHandler(struct selector_key *key);
void writeSocketHandler(struct selector_key *key);

//Los sockets activos van a compartir buffers con su "par"
typedef struct bufferAndFd{
    int fd;
    buffer * wBuff;
    buffer * rBuff;
} bufferAndFd;

//La idea es que se acceda al buffer usando el fd del socket
bufferAndFd* socksBuffer[1024];
char auxBuff[1024] = {0};

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
    snprintf(auxBuff, RW_AMOUNT, "Leo: %s\n");
    int count = recv(key->fd, auxBuff, RW_AMOUNT, MSG_DONTWAIT);
    /*
    struct bufferAndFd* b = (bufferAndFd*)key->data;
    char temp[1024];
    int nread;
    if( buffer_can_write() ){
        nread = recv(b->fd,temp,1024,MSG_DONTWAIT);
        buffer_write(nread,temp);
        selector_register(key->s, newSocket->fd, &readSocketHandler,OP_READ,&newSocket);
    }
    */
}


void writeSocketHandler(struct selector_key * key){
    snprintf(auxBuff, RW_AMOUNT, "Escribo: %s\n");
    int count = send(key->fd, auxBuff, RW_AMOUNT, MSG_DONTWAIT);
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
//TODO: ver de reserver un bufferAndFd por cada socket anidando los buffers
void tcpConnectionHandler(struct selector_key *key){

    //Abro socket para comunicarme con el cliente
    struct sockaddr_in cliSockAddr;
    socklen_t cliSockAddrSize= sizeof(cliSockAddr);

    //TODO: asegurar que el accept no bloquee
    int cliSockFd = accept(key->fd, &cliSockAddr, &cliSockAddrSize);


    selector_register(key->s, cliSockFd, &activeFdHandler, OP_READ | OP_WRITE, key->data);
    printf("Registrado el socket cliente con fd: %d\n", cliSockFd);


    //Abro socket para comunicarme con el server
    int serSockFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serSockAddr = {.sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
        .sin_port = htons(OTHER_PORT)};

    connect(serSockFd, &serSockAddr, sizeof(serSockAddr));

    selector_register(key->s, serSockFd, &activeFdHandler, OP_READ | OP_WRITE, key->data);
    printf("Registrado el socket servidor con fd: %d\n", serSockFd);
}


int main(){

	int opt = TRUE;
    int masterSocket[2];
    int masterSocketSize=0;
    struct sockaddr_in address;

	fd_set readfds;
	fd_set writefds;

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
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
        address.sin_port = htons(PORT);

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

    size_t  initial_elements = 1 ;
    fd_selector fdSelector = selector_new(initial_elements);

    selector_register(fdSelector, masterSocket[0], &passiveFdHandler, OP_READ, NULL);

    while(1){
        selector_select(fdSelector);
    }



    error:
        return -1;
}