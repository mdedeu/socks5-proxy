#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include "selector.h"
#include "state/stm.h"
#include "buffer.h"
#include "bufferService.h"
#include "sock_client.h"
#include "general_handlers.h"
#include "state/proxy_state_machine.h"
#include "parsing/sock_hello_parser.h"
#include "parsing/sock_request_parser.h"
#include "parsing/sock_authentication_parser.h"

#define MAX_PENDING_CONNECTIONS 500    // un valor bajo, para realizar pruebas
#define PORT 1080
#define OTHER_PORT 9090
#define TRUE 1
#define RW_AMOUNT 30

void tcpConnectionHandler(struct selector_key *key);
void readSocketHandler(struct selector_key *key);
void writeSocketHandler(struct selector_key *key);

//Handlers estandares para sockets activos y pasivos
fd_handler passiveFdHandler = {
        .handle_read = &tcpConnectionHandler,
        .handle_write = NULL,
        .handle_block = NULL,
        .handle_close = NULL
};

//to be replaced by the state machine methods
fd_handler activeFdHandler = {
    .handle_read = &socks5_read,
    .handle_write = &socks5_write,
    .handle_block = &socks5_block,
    .handle_close = &socks5_close
};

//
//void readSocketHandler(struct selector_key *key){
//
//    bufferAndFd * bufferAndFd;
//    size_t readAmount;
//
//    if(( bufferAndFd = getBufferAndFd(key->fd) ) == NULL)
//        return;
//
//    buffer * writeBuffer = bufferAndFd->wBuff;
//
//    if(!buffer_can_write(writeBuffer))
//        return;
//
//    buffer_compact(writeBuffer);
//    uint8_t * writePtr = buffer_write_ptr(writeBuffer, &readAmount);
//    ssize_t readBytes = recv(key->fd, writePtr, readAmount, MSG_DONTWAIT);
//    buffer_write_adv(writeBuffer, readBytes);
//
//}
//
//
//void writeSocketHandler(struct selector_key * key){
//    bufferAndFd * bufferAndFd;
//    size_t writeAmount;
//
//    if(( bufferAndFd = getBufferAndFd(key->fd) ) == NULL)
//        return;
//
//    buffer * readBuffer = bufferAndFd->rBuff;
//
//    if(!buffer_can_read(readBuffer))
//        return;
//
//    uint8_t * readPtr = buffer_read_ptr(readBuffer, &writeAmount);
//    ssize_t writtenBytes = send(key->fd, readPtr, writeAmount, MSG_DONTWAIT);
//    buffer_read_adv(readBuffer, writtenBytes);
//    buffer_compact(readBuffer);
//
//}

//El cliente se conecta conmigo a traves del socket pasivo tcp (master_socket)
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
    int cliSockFd = accept(key->fd, (struct sockaddr *) &cliSockAddr, &cliSockAddrSize);

    struct sock_client * new_client = init_new_client_connection(cliSockFd);
    selector_register(key->s, cliSockFd, &activeFdHandler, OP_READ, new_client);


//    //Abro socket para comunicarme con el server
//    int serSockFd = socket(AF_INET, SOCK_STREAM, 0);
//    struct sockaddr_in serSockAddr = {.sin_family = AF_INET,
//        .sin_addr.s_addr = inet_addr("127.0.0.1"),
//        .sin_port = htons(OTHER_PORT)};
//
//    //TODO: do not block server
//    connect(serSockFd, (struct sockaddr *) &serSockAddr, sizeof(serSockAddr));
//
//    //this only receives data, do not need an associated state machine
//    selector_register(key->s, serSockFd, &activeFdHandler, OP_READ | OP_WRITE, key->data);
//
//    crossLinkBuffers(cliSockFd, serSockFd);
}

int main(){

	int opt = TRUE;
    int master_socket[2];
    int master_socket_size=0;
    struct sockaddr_in address;

    if( (master_socket[master_socket_size] = socket(AF_INET , SOCK_STREAM , 0)) < 0)
    {
        goto error;
    } else {
        if( setsockopt(master_socket[master_socket_size], SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
        {
            goto error;
        }

        selector_fd_set_nio(master_socket[master_socket_size]);

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
        address.sin_port = htons(PORT);

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

    struct sockaddr_in6 server6addr;
	if ((master_socket[master_socket_size] = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
	{
        goto error;
	} else {
		if (setsockopt(master_socket[master_socket_size], SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0)
		{
            goto error;
		}
        selector_fd_set_nio(master_socket[master_socket_size]);

		memset(&server6addr, 0, sizeof(server6addr));
		server6addr.sin6_family = AF_INET6;
		server6addr.sin6_port = htons(PORT);
        inet_pton(AF_INET6, "::1", &server6addr.sin6_addr);

		if (bind(master_socket[master_socket_size], (struct sockaddr *) &server6addr, sizeof(server6addr)) < 0)
		{
			close(master_socket[master_socket_size]);
            goto error;
		} else {
			if (listen(master_socket[master_socket_size], MAX_PENDING_CONNECTIONS) < 0)
			{
				close(master_socket[master_socket_size]);
                goto error;
			} else {
				master_socket_size++;
			}
		}
	}

    const struct selector_init conf = {
            .signal = SIGALRM,
            .select_timeout = {
                    .tv_sec  = 10,
                    .tv_nsec = 0,
            },
    };

//    struct selector_init selector_initializer = {.select_timeout = tp, .signal = SIGALRM};
    selector_init(&conf);

    fd_selector fdSelector = selector_new(1024);

    selector_register(fdSelector, master_socket[1], &passiveFdHandler, OP_READ, NULL);
    selector_register(fdSelector, master_socket[0], &passiveFdHandler, OP_READ, NULL);

    while(1){
        selector_select(fdSelector);
    }

    error:
        return -1;
}