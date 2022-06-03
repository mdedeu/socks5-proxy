#include "stm.c"
#include <string.h>
#include "sock_hello_parser.c"
#include "selector.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "bufferService.h"

#define READ_AMOUNT 512

enum sock_state{
    TCP_CONNECTED,
    HELLO_SOCK_RECEIVED,
    AUTHENTICATED,
    CONNECT_SOCK_RECEIVED,
    CONNECTED
};

fd_handler on_tcp_connected_fd_handler= {
        .handle_read = NULL,
        .handle_write = &on_tcp_connected_handler_write,
        .handle_block = NULL,
        .handle_close = NULL 
};


//return the new state if corresponding
//should read from the fd associated (key->fd) and give the data read to the corresponding parser.
// How to save the parser for the current client??
static unsigned on_tcp_connected_handler_read(struct selector_key * key){

    key->data = init_sock_hello_parser();

    char aux_buff[READ_AMOUNT];
    int read_amount = recv(key->fd, aux_buff, READ_AMOUNT, MSG_DONTWAIT);

    bool finished = feed_sock_hello_parser((struct sock_hello_message *) key->data, aux_buff, read_amount);

    selector_register(key->s, key->fd, &on_tcp_connected_fd_handler, OP_WRITE, key->data);
}

static unsigned hello_sock_received_handler_read(struct selector_key * key);
static unsigned authenticated_handler_read(struct selector_key * key);
static unsigned connect_sock_received_handler_read(struct selector_key * key);
static unsigned connected_handler_read(struct selector_key * key);

//return the new state if corresponding
static unsigned on_tcp_connected_handler_write(struct selector_key * key){

    bufferAndFd * bufferAndFd;
    size_t writeAmount;

    if(( bufferAndFd = getBufferAndFd(key->fd) ) == NULL)
        return TCP_CONNECTED;

    buffer * readBuffer = bufferAndFd->rBuff;

    if(!buffer_can_read(readBuffer))
        return TCP_CONNECTED;

    uint8_t * readPtr = buffer_read_ptr(readBuffer, &writeAmount);
    ssize_t writtenBytes = send(key->fd, readPtr, writeAmount, MSG_DONTWAIT);
    buffer_read_adv(readBuffer, writtenBytes);
    buffer_compact(readBuffer);
}

static unsigned hello_sock_received_handler_write(struct selector_key * key);
static unsigned authenticated_handler_write(struct selector_key * key);
static unsigned connect_sock_received_handler_write(struct selector_key * key);
static unsigned connected_handler_write(struct selector_key * key);


static struct state_definition tcp_connected_state = {.state=TCP_CONNECTED,.on_read_ready=on_tcp_connected_handler_read,.on_write_ready=on_tcp_connected_handler_write};
static struct state_definition hello_sock_received_state= {.state=HELLO_SOCK_RECEIVED,.on_read_ready=hello_sock_received_state,.on_write_ready=hello_sock_received_handler_write};
static struct state_definition authenticated_state = {.state=AUTHENTICATED,.on_read_ready=authenticated_handler_read,.on_write_ready=authenticated_handler_write};
static struct state_definition connect_sock_received_state = {.state=CONNECT_SOCK_RECEIVED,.on_read_ready=connect_sock_received_handler_read,.on_write_ready=connect_sock_received_handler_write};
static struct state_definition connected_state = {.state=CONNECTED,.on_read_ready=connected_handler_read,.on_write_ready=connected_handler_write};

static struct state_definition states[]={tcp_connected_state,hello_sock_received_state,authenticated_state,connect_sock_received_state,connected_state};

static struct state_machine sock_client_machine={
        .initial=TCP_CONNECTED,
        .states=states,
        .max_state = N(states)
};

struct state_machine * init_proxy_state_machine(){
    struct state_machine * aux = malloc(sizeof (state_machine));
    memcpy(aux,&sock_client_machine,sizeof (state_machine));
    struct state_machine  * current = stm_init(&aux);
}

void destroy_sock_state(struct state_machine * sock_machine){
    free(sock_machine);
}
