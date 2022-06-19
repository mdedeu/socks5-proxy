#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../sock_client.h"
#include "../cool_client.h"
#include "../cool_handlers.h"
#include "../args.h"



#define MAX_PENDING_CONNECTIONS 100
#define TRUE 1
#define SOCKS_PASSIVE_SOCKET_SIZE 2
#define SOCKS_COOL_PASSIVE_SOCKET_SIZE 2
#define DEFAULT_SOCK_PORT 1080
#define COOL_PORT 42069

#define IPV4_LOOPBACK_STRING "127.0.0.1"
#define IPV6_LOOPBACK_STRING "::1"
#define IPV4_ADDR_ANY_STRING "0.0.0.0"
#define IPV6_ADDR_ANY_STRING "::"

static bool done = false;

static void
sigterm_handler(const int signal) {
    printf("signal %d, cleaning up and exiting\n",signal);
    done = true;
}


static void tcpConnectionHandler(struct selector_key * key,struct sockaddr * client_address , socklen_t *client_address_len,bool ipv4);
void tcpConnectionHandlerIpv4(struct selector_key *key);
void tcpConnectionHandlerIpv6(struct selector_key *key);
void coolTcpConnectionHandler(struct selector_key *key);


const fd_handler passive_handlers_ipv4 = {
        .handle_read = &tcpConnectionHandlerIpv4,
        .handle_write = NULL,
        .handle_block = NULL,
        .handle_close = NULL,
        .handle_timeout = NULL
};

const fd_handler passive_handlers_ipv6 = {
        .handle_read = &tcpConnectionHandlerIpv6,
        .handle_write = NULL,
        .handle_block = NULL,
        .handle_close = NULL,
        .handle_timeout = NULL
};

const fd_handler passive_handlers[2] = {passive_handlers_ipv4,passive_handlers_ipv6};

fd_handler active_handlers = {
    .handle_read = &socks5_read,
    .handle_write = &socks5_write,
    .handle_block = &socks5_block,
    .handle_close = NULL,
    .handle_timeout=&socks5_timeout
};

fd_handler cool_passive_handlers = {
        .handle_read = &coolTcpConnectionHandler,
        .handle_write = NULL,
        .handle_block = NULL,
        .handle_close = NULL,
        .handle_timeout = NULL
};

fd_handler cool_active_handlers = {
    .handle_read = &cool_read,
    .handle_write = &cool_write,
    .handle_block = &cool_block,
    .handle_close = NULL,
    .handle_timeout=&cool_timeout
};



void tcpConnectionHandlerIpv4(struct selector_key *key){
    struct sockaddr_in  new_client_information;
    socklen_t  new_client_information_size =  sizeof(new_client_information);
    tcpConnectionHandler(key,(struct sockaddr*)&new_client_information , &new_client_information_size,true);
}

void tcpConnectionHandlerIpv6(struct selector_key *key){
    struct sockaddr_in6  new_client_information;
    socklen_t  new_client_information_size =  sizeof(new_client_information);
    tcpConnectionHandler(key,(struct sockaddr*)&new_client_information , &new_client_information_size,false);
}

static void tcpConnectionHandler(struct selector_key * key,struct sockaddr * client_address , socklen_t * client_address_len,bool ipv4){
    int new_client_fd;
    do{
        new_client_fd = accept(key->fd,  client_address, client_address_len);
    }while(new_client_fd < 0 && (errno == EINTR) );
    struct sock_client * new_client_data = init_new_client_connection(new_client_fd,client_address,ipv4);
    if(new_client_data != NULL)
        if( SELECTOR_SUCCESS != selector_register(key->s, new_client_fd, &active_handlers, OP_READ, new_client_data) ){
            destroy_sock_client(new_client_data);
            close(new_client_fd);
        }
}


void coolTcpConnectionHandler(struct selector_key *key){
    struct sockaddr  new_client_information;
    socklen_t  new_client_information_size =  sizeof(new_client_information);

    int new_client_fd = accept(key->fd,  &new_client_information, &new_client_information_size);
    if(new_client_fd > 0 ){
        struct cool_client * new_client_data = init_cool_client_connection(new_client_fd);
        if(new_client_data != NULL)
            if(SELECTOR_SUCCESS != selector_register(key->s, new_client_fd, &cool_active_handlers, OP_READ, new_client_data) )
                destroy_cool_client(new_client_data);
    }
}

int main(const int argc,  char **argv){

    signal(SIGTERM, sigterm_handler);
    signal(SIGINT,  sigterm_handler);
    int ret = 0;
    unsigned port;
    bool could_bind = false;

    struct socks5args received_args;
    parse_args(argc, argv, &received_args);


    /*initializing metrics.h*/
    port = received_args.socks_port;
    if(!received_args.disectors_enabled)
       disable_spoofing_handler();

    set_clients_need_authentication(received_args.disectors_enabled);

    for(int i = 0 ; i < 10 ; i ++){
        if(received_args.users[i].name != NULL && received_args.users[i].pass != NULL )
        add_user_handler(strlen(received_args.users[i].name),(uint8_t * )received_args.users[i].name,
                         strlen(received_args.users[i].pass),(uint8_t*)received_args.users[i].pass);
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

    if(!strcmp(received_args.socks_addr, IPV4_ADDR_ANY_STRING) || !strcmp(received_args.socks_addr, IPV6_ADDR_ANY_STRING))
        received_args.socks_addr = IPV4_ADDR_ANY_STRING;

    inet_pton(AF_INET, received_args.socks_addr, &server_address_4.sin_addr.s_addr);
    server_address_4.sin_port        = htons(port);

    if( (master_socket[current_sock_passive_socket] = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP)) < 0)
    {
        err_msg = "Unable to create socket for ipv4 socks5";
        goto finally;
    }

    int opt = TRUE;
    setsockopt(master_socket[current_sock_passive_socket], SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) ;


    if (bind(master_socket[current_sock_passive_socket], (struct sockaddr *)&server_address_4, sizeof(server_address_4))<0)
        {
            err_msg = "Unable to bind socket for ipv4 socks5";
            fprintf(stdout, "%s\n", err_msg);
            could_bind = could_bind || false;
        }
    else{
        could_bind = could_bind || true;
        fprintf(stdout, "Listening on TCP port %u for ipv4 socks5\n", port);
    }

    if (listen(master_socket[0], MAX_PENDING_CONNECTIONS) < 0){
        err_msg = "Unable to listen for ipv4 socks5";
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
        err_msg = "Unable to create socket for ipv6 socks5";
        goto finally;
	}
    setsockopt(master_socket[current_sock_passive_socket], SOL_IPV6, IPV6_V6ONLY, (char *) &opt, sizeof(opt));

    struct sockaddr_in6 server_address_6;
    memset(&server_address_6, 0, sizeof(server_address_6));
	server_address_6.sin6_family = AF_INET6;
	server_address_6.sin6_port = htons(port);
    if(!strcmp(received_args.socks_addr, IPV4_ADDR_ANY_STRING) || !strcmp(received_args.socks_addr, IPV6_ADDR_ANY_STRING))
        received_args.socks_addr = IPV6_ADDR_ANY_STRING;

    if (bind(master_socket[current_sock_passive_socket], (struct sockaddr *) &server_address_6, sizeof(server_address_6)) < 0)
	{
        err_msg = "Unable to bind for ipv6 socks5";
        fprintf(stdout, "%s\n", err_msg);
        could_bind = could_bind || false;
	}
    else{
        could_bind = could_bind || true;
        fprintf(stdout, "Listening on TCP port %u for ipv6 socks5\n", port);
    }
    
    if(!could_bind){
        err_msg = "Unable to bind for socks5";
        goto finally;
    }


    if (listen(master_socket[current_sock_passive_socket], MAX_PENDING_CONNECTIONS) < 0)
		{
            err_msg = "Unable to listen for ipv6 socks5";
            goto finally;
		}

    if(selector_fd_set_nio(master_socket[current_sock_passive_socket]) == -1) {
        err_msg = "getting server socket flags";
        goto finally;
    }
    current_sock_passive_socket++;

    //=======================COOL_IPV4==============================

    could_bind = false;
    memset(&server_address_4, 0, sizeof(server_address_4));
    server_address_4.sin_family = AF_INET;

    if(!strcmp(received_args.mng_addr, IPV4_LOOPBACK_STRING) || !strcmp(received_args.mng_addr, IPV6_LOOPBACK_STRING))
        received_args.mng_addr = IPV4_LOOPBACK_STRING;

    inet_pton(AF_INET, received_args.mng_addr, &server_address_4.sin_addr.s_addr);
    server_address_4.sin_port = htons(received_args.mng_port);

    if( (cool_master_socket[current_sock_cool_passive_socket] = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP)) < 0)
    {
        err_msg = "Unable to create socket for ipv4 management";
        goto finally;
    }

    setsockopt(cool_master_socket[current_sock_cool_passive_socket], SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) ;

    if (bind(cool_master_socket[current_sock_cool_passive_socket], (struct sockaddr *)&server_address_4, sizeof(server_address_4))<0)
        {
            err_msg = "Unable to bind socket for ipv4 management";
            fprintf(stdout, "%s\n", err_msg);
            could_bind = could_bind || false;
        }
    else
        could_bind = could_bind || true;

    if (listen(cool_master_socket[0], MAX_PENDING_CONNECTIONS) < 0){
        err_msg = "Unable to listen for ipv4 management";
        goto finally;
    }
    else
        fprintf(stdout, "Listening on TCP port %u for ipv4 management\n", received_args.mng_port);

    if(selector_fd_set_nio(cool_master_socket[current_sock_cool_passive_socket]) == -1) {
        err_msg = "getting server socket flags";
        goto finally;
    }
    current_sock_cool_passive_socket++;

    //=========================COOL_IPV6=================================

	if ((cool_master_socket[current_sock_cool_passive_socket] = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
        err_msg = "Unable to create socket for ipv6 management";
        goto finally;
	}
    setsockopt(cool_master_socket[current_sock_cool_passive_socket], SOL_IPV6, IPV6_V6ONLY, (char *) &opt, sizeof(opt));

    memset(&server_address_6, 0, sizeof(server_address_6));
	server_address_6.sin6_family = AF_INET6;
	server_address_6.sin6_port = htons(received_args.mng_port);

    if(!strcmp(received_args.mng_addr, IPV4_LOOPBACK_STRING) || !strcmp(received_args.mng_addr, IPV6_LOOPBACK_STRING))
        received_args.mng_addr = IPV6_LOOPBACK_STRING;

    inet_pton(AF_INET6, received_args.mng_addr, &server_address_6.sin6_addr);

	if (bind(cool_master_socket[current_sock_cool_passive_socket], (struct sockaddr *) &server_address_6, sizeof(server_address_6)) < 0)
	{
        err_msg = "Unable to bind for ipv6 management";
        fprintf(stdout, "%s\n", err_msg);
        could_bind = could_bind || false;
	}
    else{
        could_bind = could_bind || true;
    }

    if(!could_bind){
        err_msg = "Unable to bind for management";
        goto finally;
    }

    if (listen(cool_master_socket[current_sock_cool_passive_socket], MAX_PENDING_CONNECTIONS) < 0)
		{
            err_msg = "Unable to listen for ipv6 management";
            goto finally;
		}
    else
        fprintf(stdout, "Listening on TCP port %u for ipv6 management\n", received_args.mng_port);

    if(selector_fd_set_nio(cool_master_socket[current_sock_cool_passive_socket]) == -1) {
        err_msg = "getting server socket flags";
        goto finally;
    }
    current_sock_cool_passive_socket++;


    const struct selector_init conf = {
        .signal = SIGALRM,
        .select_timeout = {
                .tv_sec  = 1,
                .tv_nsec = 0,
        },
    };

    if(0 != selector_init(&conf)) {
        err_msg = "initializing selector";
        goto finally;
    }

    selector = selector_new(FD_SETSIZE);
    if(selector == NULL) {
        err_msg = "Unable to create selector";
        goto finally;
    }

    for(long unsigned int  i = 0 ; i < N(master_socket) ; i++){
        selector_status_returned = selector_register(selector, master_socket[i], &passive_handlers[i], OP_READ, NULL);
        if(SELECTOR_SUCCESS != selector_status_returned ){
            err_msg = "registering fd";
            goto finally;
        }
    }

    for(long unsigned int  i = 0 ; i < N(cool_master_socket) ; i++){
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

        for(long unsigned int  i = 0  ;i < N(master_socket); i++){
            if(master_socket[i] > 0 )
                close(master_socket[i]);
        }

        for(long unsigned int  i = 0; i < N(cool_master_socket); i++){
            if(cool_master_socket[i] > 0 )
                close(cool_master_socket[i]);
        }

        remove_all_users();

        return ret;
}