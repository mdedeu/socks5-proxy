#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "selector.h"
#include "sock_client.h"
#include "cool_client.h"
#include "general_handlers.h"
#include "cool_handlers.h"
#include "parsing/sock_request_parser.h"
#include <limits.h>



#define MAX_PENDING_CONNECTIONS 500
#define OTHER_PORT 9090
#define COOL_PORT 42069
#define TRUE 1
#define RW_AMOUNT 30
#define SOCKS_PASSIVE_SOCKET_SIZE 2
#define SOCKS_COOL_PASSIVE_SOCKET_SIZE 1
#define DEFAULT_SOCK_PORT 1080

static bool done = false;

static void
sigterm_handler(const int signal) {
    printf("signal %d, cleaning up and exiting\n",signal);
    done = true;
}



void tcpConnectionHandler(struct selector_key *key);
void coolTcpConnectionHandler(struct selector_key *key);


fd_handler passive_handlers = {
        .handle_read = &tcpConnectionHandler,
        .handle_write = NULL,
        .handle_block = NULL,
        .handle_close = NULL
};

fd_handler active_handlers = {
    .handle_read = &socks5_read,
    .handle_write = &socks5_write,
    .handle_block = &socks5_block,
    .handle_close = NULL/*&socks5_close*/
};

fd_handler cool_passive_handlers = {
        .handle_read = &coolTcpConnectionHandler,
        .handle_write = NULL,
        .handle_block = NULL,
        .handle_close = NULL
};

fd_handler cool_active_handlers = {
    .handle_read = &cool_read,
    .handle_write = &cool_write,
    .handle_block = &cool_block,
    .handle_close = &cool_close
};


void tcpConnectionHandler(struct selector_key *key){
    struct sockaddr  new_client_information;
    socklen_t  new_client_information_size =  sizeof(new_client_information);

    int new_client_fd = accept(key->fd,  &new_client_information, &new_client_information_size);
    if(new_client_fd > 0 ){
        struct sock_client * new_client_data = init_new_client_connection(new_client_fd,&new_client_information);
        if(new_client_data != NULL)
            selector_register(key->s, new_client_fd, &active_handlers, OP_READ, new_client_data);
    }
}

void coolTcpConnectionHandler(struct selector_key *key){
    struct sockaddr  new_client_information;
    socklen_t  new_client_information_size =  sizeof(new_client_information);

    int new_client_fd = accept(key->fd,  &new_client_information, &new_client_information_size);
    if(new_client_fd > 0 ){
        struct cool_client * new_client_data = init_cool_client_connection(new_client_fd);
        if(new_client_data != NULL)
            selector_register(key->s, new_client_fd, &cool_active_handlers, OP_READ, new_client_data);
    }
}

int main(const int argc, const char **argv){

    signal(SIGTERM, sigterm_handler);
    signal(SIGINT,  sigterm_handler);
    int ret ;
    unsigned port = DEFAULT_SOCK_PORT;

    if(argc == 1) {
        // utilizamos el default
    } else if(argc == 2) {
        char *end     = 0;
        const long sl = strtol(argv[1], &end, 10);

        if (end == argv[1]|| '\0' != *end
            || ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno)
            || sl < 0 || sl > USHRT_MAX) {
            fprintf(stderr, "port should be an integer: %s\n", argv[1]);
            return 1;
        }
        port = sl;
    } else {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    close(STDIN_FILENO);

    const char       *err_msg = NULL;
    fd_selector selector      = NULL;
    selector_status   selector_status_returned      = SELECTOR_SUCCESS;


    int master_socket[SOCKS_PASSIVE_SOCKET_SIZE];
    int cool_master_socket[SOCKS_COOL_PASSIVE_SOCKET_SIZE];
    int current_sock_passive_socket=0;
    int current_sock_cool_passive_socket=0;

    //=========================IPV4=================================
    
    struct sockaddr_in server_address_4;
    memset(&server_address_4, 0, sizeof(server_address_4));
    server_address_4.sin_family      = AF_INET;
    server_address_4.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address_4.sin_port        = htons(port);

    if( (master_socket[current_sock_passive_socket] = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP)) < 0)
    {
        err_msg = "unable to create socket for ipv4";
        goto finally;
    }

    int opt = TRUE;
    setsockopt(master_socket[current_sock_passive_socket], SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) ;
    fprintf(stdout, "Listening on TCP port %d\n", port);


    if (bind(master_socket[current_sock_passive_socket], (struct sockaddr *)&server_address_4, sizeof(server_address_4))<0)
        {
            err_msg = "unable to bind socket for ipv4";
            goto finally;
        }

    if (listen(master_socket[0], MAX_PENDING_CONNECTIONS) < 0){
        err_msg = "unable to listen for ipv4";
        goto finally;
    }

    if(selector_fd_set_nio(master_socket[current_sock_passive_socket]) == -1) {
        err_msg = "getting server socket flags";
        goto finally;
    }
    current_sock_passive_socket++;
    
    //=========================IPV6=================================

	if ((master_socket[current_sock_passive_socket] = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
        err_msg = "unable to create socket for ipv6";
        goto finally;
	}
    setsockopt(master_socket[current_sock_passive_socket], SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));

    struct sockaddr_in6 server_address_6;
    memset(&server_address_6, 0, sizeof(server_address_6));
	server_address_6.sin6_family = AF_INET6;
	server_address_6.sin6_port = htons(port);
    inet_pton(AF_INET6, "::1", &server_address_6.sin6_addr);

	if (bind(master_socket[current_sock_passive_socket], (struct sockaddr *) &server_address_6, sizeof(server_address_6)) < 0)
	{
        err_msg = "unable to bind  for ipv6";
        goto finally;
	}

    if (listen(master_socket[current_sock_passive_socket], MAX_PENDING_CONNECTIONS) < 0)
		{
            err_msg = "unable to listen  for ipv6";
            goto finally;
		}

    if(selector_fd_set_nio(master_socket[current_sock_passive_socket]) == -1) {
        err_msg = "getting server socket flags";
        goto finally;
    }
    current_sock_passive_socket++;

    //=======================COOL_IPV4==============================

    memset(&server_address_4, 0, sizeof(server_address_4));
    server_address_4.sin_family      = AF_INET;
    server_address_4.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address_4.sin_port        = htons(COOL_PORT);

    if( (cool_master_socket[current_sock_cool_passive_socket] = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP)) < 0)
    {
        err_msg = "unable to create socket for ipv4";
        goto finally;
    }

    setsockopt(cool_master_socket[current_sock_cool_passive_socket], SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) ;
    fprintf(stdout, "Listening on TCP port %d\n", COOL_PORT);


    if (bind(cool_master_socket[current_sock_cool_passive_socket], (struct sockaddr *)&server_address_4, sizeof(server_address_4))<0)
        {
            err_msg = "unable to bind socket for ipv4";
            goto finally;
        }

    if (listen(cool_master_socket[0], MAX_PENDING_CONNECTIONS) < 0){
        err_msg = "unable to listen for ipv4";
        goto finally;
    }

    if(selector_fd_set_nio(cool_master_socket[current_sock_cool_passive_socket]) == -1) {
        err_msg = "getting server socket flags";
        goto finally;
    }
    current_sock_cool_passive_socket++;
    
    const struct selector_init conf = {
        .signal = SIGALRM,
        .select_timeout = {
                .tv_sec  = 10,
                .tv_nsec = 0,
        },
    };

    if(0 != selector_init(&conf)) {
        err_msg = "initializing selector";
        goto finally;
    }

    selector = selector_new(1024);
    if(selector == NULL) {
        err_msg = "unable to create selector";
        goto finally;
    }

    for(int i = 0 ; i < N(master_socket) ; i++){
        selector_status_returned = selector_register(selector, master_socket[i], &passive_handlers, OP_READ, NULL);
        if(SELECTOR_SUCCESS != selector_status_returned ){
            err_msg = "registering fd";
            goto finally;
        }
    }

    for(int i = 0 ; i < N(cool_master_socket) ; i++){
        selector_status_returned = selector_register(selector, cool_master_socket[i], &cool_passive_handlers, OP_READ, NULL);
        if(SELECTOR_SUCCESS != selector_status_returned ){
            err_msg = "registering fd";
            goto finally;
        }
    }

    while(!done){
        err_msg = NULL;
        selector_status_returned = selector_select(selector);
        if( SELECTOR_SUCCESS != selector_status_returned) {
            err_msg = "serving";
            goto finally;
        }
    }

    if(err_msg == NULL) {
        err_msg = "closing";
    }


finally:
    if(selector_status_returned != SELECTOR_SUCCESS) {
            fprintf(stderr, "%s: %s\n", (err_msg == NULL) ? "": err_msg,
                    selector_status_returned == SELECTOR_IO
                        ? strerror(errno)
                        : selector_error(selector_status_returned));
                ret = 2;
            } else if(err_msg) {
                perror(err_msg);
                ret = 1;
        }

        if(selector != NULL){
            selector_destroy(selector);
        }
        selector_close();

        for(int i = 0  ;i < N(master_socket); i++){
            if(master_socket[i] > 0 )
                close(master_socket[i]);
        }

        for(int i = 0; i < N(cool_master_socket); i++){
            if(cool_master_socket[i] > 0 )
                close(cool_master_socket[i]);
        }

        return ret;
}