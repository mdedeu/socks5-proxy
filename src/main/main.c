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



#define MAX_PENDING_CONNECTIONS 20
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
static int new_listen_socket(struct sockaddr *addr,socklen_t addrLen );
static int socks_ipv4_listen(unsigned  int port);
static int socks_ipv6_listen(unsigned  int port);


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
    int i = 0 ;
    while(i < MAX_PENDING_CONNECTIONS){
        new_client_fd = accept(key->fd,  client_address, client_address_len);
        if(new_client_fd  < 0 &&  errno == EWOULDBLOCK)
            break; //no more pending connections
        else if(new_client_fd >= 0 ){
            struct sock_client * new_client_data = init_new_client_connection(new_client_fd,client_address,ipv4);
            if(new_client_data != NULL){
                if( SELECTOR_SUCCESS != selector_register(key->s, new_client_fd, &active_handlers, OP_READ, new_client_data) ){
                    //no more capacity on select
                    destroy_sock_client(new_client_data);
                    close(new_client_fd);
                };
            }else break; //no more memory
        }
        else i++;
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


const char       *err_msg = NULL;
struct sockaddr_in server_address_4;
struct sockaddr_in6 server_address_6;




int main(const int argc,  char **argv){

    signal(SIGTERM, sigterm_handler);
    signal(SIGINT,  sigterm_handler);
    int ret = 0;
//    bool could_bind = false;

    struct socks5args received_args;
    parse_args(argc, argv, &received_args);


    /*initializing metrics.h*/
    if(!received_args.disectors_enabled)
       disable_spoofing_handler();

    //by default asking for authentication
    set_clients_need_authentication(true);

    for(int i = 0 ; i < MAX_USERS ; i ++){
        if(received_args.users[i].name != NULL && received_args.users[i].pass != NULL )
        add_user_handler(strlen(received_args.users[i].name),(uint8_t * )received_args.users[i].name,
                         strlen(received_args.users[i].pass),(uint8_t*)received_args.users[i].pass);
    }


    close(STDIN_FILENO);

    fd_selector selector      = NULL;
    selector_status   selector_status_returned      = SELECTOR_SUCCESS;


    int master_socket[SOCKS_PASSIVE_SOCKET_SIZE];
    long unsigned int current_sock_passive_socket=0;
    memset(&server_address_4, 0, sizeof(server_address_4));
    memset(&server_address_6, 0, sizeof(server_address_6));

    if(received_args.socks_addr == NULL){
        server_address_4.sin_addr.s_addr = htonl(INADDR_ANY);
        server_address_6.sin6_addr= in6addr_any;
        master_socket[current_sock_passive_socket]= socks_ipv4_listen(received_args.socks_port);
        if(master_socket[current_sock_passive_socket] >= 0 ){
            fprintf(stdout, "Listening on TCP port: %u for ipv4 socks protocol\n", received_args.socks_port);
            current_sock_passive_socket++;
        }
        master_socket[current_sock_passive_socket]= socks_ipv6_listen(received_args.socks_port);
        if(master_socket[current_sock_passive_socket] >= 0 ){
            fprintf(stdout, "Listening on TCP port: %u for ipv6 socks protocol\n", received_args.socks_port);
            current_sock_passive_socket++;
        }
    }else if(inet_pton(AF_INET, received_args.socks_addr, &server_address_4.sin_addr.s_addr)){
        master_socket[current_sock_passive_socket]= socks_ipv4_listen(received_args.socks_port);
        if(master_socket[current_sock_passive_socket] >= 0 ){
            fprintf(stdout, "Listening on TCP port: %u for ipv4 socks protocol\n", received_args.socks_port);
            current_sock_passive_socket++;
        }
    }else if(inet_pton(AF_INET6, received_args.socks_addr, &server_address_6.sin6_addr)){
        master_socket[current_sock_passive_socket]= socks_ipv6_listen(received_args.socks_port);
        if(master_socket[current_sock_passive_socket] >= 0 ){
            fprintf(stdout, "Listening on TCP port: %u for ipv6 socks protocol\n", received_args.socks_port);
            current_sock_passive_socket++;
        }
    }

    if(current_sock_passive_socket == 0 ){
        err_msg = "Unable to listen for socks5 protocol\n";
        goto  finally;
    }

    //=======================COOL_IPV4==============================
    int cool_master_socket[SOCKS_COOL_PASSIVE_SOCKET_SIZE];
    long unsigned int current_sock_cool_passive_socket=0;

    if(received_args.mng_addr == NULL){
        inet_pton(AF_INET, IPV4_LOOPBACK_STRING, &server_address_4.sin_addr.s_addr);
        inet_pton(AF_INET6, IPV6_LOOPBACK_STRING, &server_address_6.sin6_addr);
        cool_master_socket[current_sock_cool_passive_socket] = socks_ipv4_listen(received_args.mng_port);
        if(cool_master_socket[current_sock_cool_passive_socket] >= 0 ){
            fprintf(stdout, "Listening on TCP port: %u for ipv4 cool protocol\n", received_args.mng_port);
            current_sock_cool_passive_socket++;
        }
        cool_master_socket[current_sock_cool_passive_socket]= socks_ipv6_listen(received_args.mng_port);
        if(cool_master_socket[current_sock_cool_passive_socket] >= 0 ){
            fprintf(stdout, "Listening on TCP port: %u for ipv6 cool protocol\n", received_args.mng_port);
            current_sock_cool_passive_socket++;
        }
    }else if( inet_pton(AF_INET, received_args.mng_addr, &server_address_4.sin_addr.s_addr) ){
        cool_master_socket[current_sock_cool_passive_socket] = socks_ipv4_listen(received_args.mng_port);
        if(cool_master_socket[current_sock_cool_passive_socket] >= 0 ){
            fprintf(stdout, "Listening on TCP port: %u for ipv4 cool protocol\n", received_args.mng_port);
            current_sock_cool_passive_socket++;
        }
    }else if( inet_pton(AF_INET6, received_args.mng_addr, &server_address_6.sin6_addr) ){
        cool_master_socket[current_sock_cool_passive_socket]= socks_ipv6_listen(received_args.mng_port);
        if(cool_master_socket[current_sock_cool_passive_socket] >= 0 ){
            fprintf(stdout, "Listening on TCP port: %u for ipv6 cool protocol\n", received_args.mng_port);
            current_sock_cool_passive_socket++;
        }
    }

    if(current_sock_cool_passive_socket == 0 ){
        err_msg = "Unable to listen for cool protocol\n";
        goto  finally;
    }

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

    for(long unsigned int  i = 0 ; i < current_sock_passive_socket ; i++){
        selector_status_returned = selector_register(selector, master_socket[i], &passive_handlers[i], OP_READ, NULL);
        if(SELECTOR_SUCCESS != selector_status_returned ){
            err_msg = "registering fd";
            goto finally;
        }
    }

    for(long unsigned int  i = 0 ; i < current_sock_cool_passive_socket ; i++){
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

static int socks_ipv4_listen(unsigned  int port){
    server_address_4.sin_family      = AF_INET;
    server_address_4.sin_port        = htons(port);
    return new_listen_socket((struct sockaddr * ) &server_address_4, sizeof(server_address_4));
}


static int socks_ipv6_listen(unsigned  int port){
    server_address_6.sin6_family = AF_INET6;
    server_address_6.sin6_port = htons(port);
    return new_listen_socket((struct sockaddr * ) &server_address_6, sizeof(server_address_6));
}



static int new_listen_socket(struct sockaddr *addr,socklen_t addrLen ){
    const int fd = socket(addr->sa_family, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        err_msg = "Unable to create socket";
        return -1;
    }

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    selector_fd_set_nio(fd);

    if(addr->sa_family == AF_INET6) {
        setsockopt(fd, SOL_IPV6, IPV6_V6ONLY, &(int){ 1 }, sizeof(int));
    }

    if(bind(fd, addr, addrLen) < 0) {
        err_msg = "Unable to bind socket";
        return -1;
    }

    if(listen(fd, MAX_PENDING_CONNECTIONS) < 0) {
        err_msg = "Unable to listen";
        return -1;
    }

    if(selector_fd_set_nio(fd) == -1) {
        err_msg = "Getting server socket flags";
        return -1;
    }
    return fd;

}
