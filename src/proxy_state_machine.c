#include "stm.c"
#include <string.h>

enum sock_state{
    TCP_CONNECTED,
    HELLO_SOCK_RECEIVED,
    AUTHENTICATED,
    CONNECT_SOCK_RECEIVED,
    CONNECTED
};


//return the new state if corresponding
//should read from the fd associated (key->fd) and give the data read to the corresponding parser.
// How to save the parser for the current client??
static unsigned on_tcp_connected_handler_read(struct selector_key * key);
static unsigned hello_sock_received_handler_read(struct selector_key * key);
static unsigned  authenticated_handler_read(struct selector_key * key);
static unsigned connect_sock_received_handler_read(struct selector_key * key);
static unsigned connected_handler_read(struct selector_key * key);

//return the new state if corresponding
static unsigned on_tcp_connected_handler_write(struct selector_key * key);
static unsigned hello_sock_received_handler_write(struct selector_key * key);
static unsigned  authenticated_handler_write(struct selector_key * key);
static unsigned connect_sock_received_handler_write(struct selector_key * key);
static unsigned connected_handler_write(struct selector_key * key);


static struct state_definition tcp_connected_state = {.state=TCP_CONNECTED,.on_read_ready=on_tcp_connected_handler_read,.on_write_ready=on_tcp_connected_handler_write};
static struct state_definition hello_sock_received_state= {.state=HELLO_SOCK_RECEIVED,.on_read_ready=hello_sock_received_state,.on_write_ready=hello_sock_received_handler_write};
static struct state_definition authenticated_state = {.state=AUTHENTICATED,.on_read_ready=authenticated_handler_read,.on_write_ready=authenticated_handler_write};
static struct state_definition connect_sock_received_state = {.state=CONNECT_SOCK_RECEIVED,.on_read_ready=connect_sock_received_handler_read,.on_write_ready=connect_sock_received_handler_write};
static struct state_definition connected_state = {.state=CONNECTED,.on_read_ready=connected_handler_read,.on_write_ready=connected_handler_write};

static struct state_definition states[]={tcp_connected_state,hello_sock_received_state,authenticated_state,connect_sock_received_state,connected_state};

struct state_machine sock_client_machine={
        .initial=TCP_CONNECTED,
        .states=states,
        .max_state = N(states)
};

struct state_machine * init_proxy_state_machine(){
    struct state_machine  * current = malloc(sizeof (struct state_machine));
    memcpy(current,&sock_client_machine,sizeof (sizeof(struct state_machine)));
    return current;
}

void destroy_sock_state(struct state_machine * sock_machine){
    free(sock_machine);
}
