#include "sock_request_parser.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

enum state_and_events{
    INITIAL_STATE,
    VERSION_READ,
    CMD_READ,
    RSV_READ,
    IPV4_ATYP_READ,
    IPV6_ATYP_READ,
    ADDR_ATYP_READ,
    ADDRLEN_READ,
    ADDR_READING,
    IPV4_READING,
    IPV6_READING,
    PORT_READING,
    END,

    VERSION_READ_EVENT,
    CMD_READ_EVENT,
    RSV_READ_EVENT,
    READ_IPV4_ATYP_EVENT,
    READ_IPV6_ATYP_EVENT,
    READ_ADDR_ATYP_EVENT,
    IPV4_READING_EVENT,
    IPV6_READING_EVENT,
    ADDR_LEN_READING_EVENT,
    ADDR_READING_EVENT,
    PORT_READING_EVENT,
    ERROR_FOUND_EVENT
};

static void request_read_version(struct  parser_event * event , uint8_t c){
    event->type = VERSION_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
static void read_cmd(struct  parser_event * event , uint8_t c){
    event->type = CMD_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void read_rsv(struct  parser_event * event , uint8_t c){
    event->type = RSV_READ_EVENT;
//    event->data[0]=c;
//    event->n=1;
}

static void read_ipv4_atyp(struct  parser_event * event , uint8_t c){
    event->type = READ_IPV4_ATYP_EVENT;
    event->data[0]=c;
    event->n=1;
}
static void read_ipv6_atyp(struct  parser_event * event , uint8_t c){
    event->type = READ_IPV6_ATYP_EVENT;
    event->data[0]=c;
    event->n=1;
}
static void read_address_atyp(struct  parser_event * event , uint8_t c){
    event->type = READ_ADDR_ATYP_EVENT;
    event->data[0]=c;
    event->n=1;
}


static void reading_ipv4(struct  parser_event * event , uint8_t c){
    event->type = IPV4_READING_EVENT;
    event->data[0]=c;
    event->n=1;
}
static void reading_ipv6(struct  parser_event * event , uint8_t c){
    event->type = IPV6_READING_EVENT;
    event->data[0]=c;
    event->n=1;
}


static void read_addr_len(struct  parser_event * event , uint8_t c){
    event->type = ADDR_LEN_READING_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void reading_addr(struct  parser_event * event , uint8_t c){
    event->type = ADDR_READING_EVENT;
    event->data[0]=c;
    event->n=1;
}


static void reading_port(struct  parser_event * event , uint8_t c){
    event->type = PORT_READING_EVENT;
    event->data[0]=c;
    event->n=1;
}


static struct parser_state_transition initial_state_transitions[] ={
        {.when=ANY,.dest=VERSION_READ,.act1=request_read_version}
};

static struct parser_state_transition version_read_transitions[] ={
        {.when=ANY,.dest=CMD_READ,.act1=read_cmd}
};

static struct parser_state_transition cmd_read_transitions[] ={
        {.when=ANY,.dest=RSV_READ,.act1=read_rsv}
};

static struct parser_state_transition rsv_read_transitions[] ={
        {.when=1,.dest=IPV4_ATYP_READ,.act1=read_ipv4_atyp},
        {.when=3,.dest=ADDR_ATYP_READ,.act1=read_address_atyp},
        {.when=4,.dest=IPV6_ATYP_READ,.act1=read_ipv6_atyp}
};

static struct parser_state_transition ipv4_atyp_read_transitions[] ={
        {.when=ANY,.dest= IPV4_READING,.act1=reading_ipv4}
};

static struct parser_state_transition ipv6_atyp_read_transitions[] ={
        {.when=ANY,.dest= IPV6_READING,.act1=reading_ipv6}
};

static struct parser_state_transition addr_atyp_read_transitions[] ={
        {.when=ANY,.dest= ADDRLEN_READ,.act1=read_addr_len}
};

static struct parser_state_transition addr_len_read_transitions[] ={
        {.when=ANY,.dest= ADDR_READING,.act1=reading_addr}
};

static struct parser_state_transition addr_reading_transitions[] ={
        {.when=ANY,.dest= ADDR_READING,.act1=reading_addr}
};

static struct parser_state_transition ipv4_reading_transitions[] ={
        {.when=ANY,.dest= IPV4_READING,.act1=reading_ipv4}
};

static struct parser_state_transition ipv6_reading_transitions[] ={
        {.when=ANY,.dest= IPV6_READING,.act1=reading_ipv6}
};

static struct parser_state_transition port_reading_transitions[]={
        {.when=ANY,.dest=PORT_READING,.act1=reading_port}
};

static const struct parser_state_transition  * sock_request_parser_transitions[] = {
        initial_state_transitions,
        version_read_transitions,
        cmd_read_transitions,
        rsv_read_transitions,
        ipv4_atyp_read_transitions,
        ipv6_atyp_read_transitions,
        addr_atyp_read_transitions,
        addr_len_read_transitions,
        addr_reading_transitions,
        ipv4_reading_transitions,
        ipv6_reading_transitions,
        port_reading_transitions
};

static const size_t  sock_request_parser_transitions_count[] = {
        N(initial_state_transitions),
        N(version_read_transitions),
        N(cmd_read_transitions),
        N(rsv_read_transitions),
        N(ipv4_atyp_read_transitions),
        N(ipv6_atyp_read_transitions),
        N(addr_atyp_read_transitions),
        N(addr_len_read_transitions),
        N(addr_reading_transitions),
        N(ipv4_reading_transitions),
        N(ipv6_reading_transitions),
        N(port_reading_transitions)
};



static struct parser_definition sock_parser_definition={
        .start_state=INITIAL_STATE,
        .states=sock_request_parser_transitions,
        .states_count=N(sock_request_parser_transitions),
        .states_n=sock_request_parser_transitions_count
};

static void handle_version_read_event(struct sock_request_message * sock_data ,uint8_t current_character){
    sock_data->version = current_character;
}
static void handle_cmd_read_event(struct parser * using_parser,struct sock_request_message * sock_data ,uint8_t current_character){
    if(current_character == 1 )
        sock_data->cmd = current_character;
    else
        using_parser->state = END;
}
static void handle_ipv4_atyp_read_event(struct parser * using_parser,struct sock_request_message * sock_data ,uint8_t current_character){
    if(current_character != 1 )
        using_parser->state = END;
    else {
        sock_data->atyp = 1 ;
        sock_data->ipv4 = malloc(IPV4SIZE);
    }
}
static void handle_ipv6_atyp_read_event(struct parser * using_parser,struct sock_request_message * sock_data ,uint8_t current_character){
    if(current_character != 4 )
        using_parser->state = END;
    else{
        sock_data->atyp = 4 ;
        sock_data->ipv6 = malloc(IPV6SIZE);
    }

}
static void handle_addr_atyp_read_event(struct parser * using_parser,struct sock_request_message * sock_data ,uint8_t current_character){
    if(current_character != 3)
         using_parser -> state = END;
    else sock_data->atyp = 3;
}
static void handle_addrlen_read_event(struct sock_request_message * sock_data ,uint8_t current_character) {
    sock_data->address = malloc(current_character + 1 ) ;
    sock_data->addrlen = current_character;
}
static void handle_ipv4_reading_event(struct parser * using_parser,struct sock_request_message * sock_data , uint8_t current_character){
    sock_data->ipv4[sock_data->ipv4_character_read] = current_character;
    sock_data->ipv4_character_read++;
    if(sock_data->ipv4_character_read == IPV4SIZE)
        using_parser->state=PORT_READING;
}
static void handle_ipv6_reading_event(struct parser * using_parser,struct sock_request_message * sock_data ,uint8_t current_character){
    sock_data->ipv6[sock_data->ipv6_character_read] = current_character;
    sock_data->ipv6_character_read++;
    if(sock_data->ipv6_character_read == IPV6SIZE)
        using_parser->state=PORT_READING;
}
static void handle_addr_reading_event(struct parser * using_parser,struct sock_request_message * sock_data ,uint8_t current_character){
    sock_data->address[sock_data->addr_character_read] = current_character;
    sock_data->addr_character_read++;
    if(sock_data->addr_character_read == sock_data->addrlen){
        sock_data->address[sock_data->addr_character_read]  = 0 ; 
        using_parser->state = PORT_READING;
    }
}
static void handle_port_reading_event(struct parser * using_parser,struct sock_request_message * sock_data ,uint8_t current_character){
    if(sock_data->port_character_read == 0 ){
        sock_data->port= malloc(2);
        sock_data->port[0]=current_character;
        sock_data->port_character_read++;
    }else{
        sock_data->port[1] = current_character;
        sock_data->port_character_read++;
        using_parser->state=END;
    }
}

struct parser * init_sock_request_parser(){
    return parser_init(parser_no_classes(),&sock_parser_definition);

}


struct sock_request_message * init_sock_request_message(){
    struct sock_request_message * new_sock_request_message = malloc(sizeof (struct sock_request_message));
    new_sock_request_message->port_character_read=0;
    new_sock_request_message->ipv6_character_read=0;
    new_sock_request_message->ipv4_character_read=0;
    new_sock_request_message->addr_character_read=0;
    new_sock_request_message->connection_result = 0;
//    new_sock_request_message->answer_buffer = malloc(sizeof (buffer));
//    buffer_init(new_sock_request_message->answer_buffer,1024,new_sock_request_message->raw_buffer);
    return new_sock_request_message;
}


bool feed_sock_request_parser(struct parser * using_parser, struct sock_request_message * sock_data, char * input, int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (using_parser->state != END  ); i++){
        current_event = parser_feed(using_parser, input[i]);
        uint8_t  current_character = current_event->data[0];
        switch (current_event->type) {
            case VERSION_READ_EVENT:
                handle_version_read_event(sock_data,current_character);
                break;
            case CMD_READ_EVENT:
                handle_cmd_read_event(using_parser,sock_data,current_character);
                break;
            case RSV_READ_EVENT:
                break;
            case READ_IPV4_ATYP_EVENT:
                handle_ipv4_atyp_read_event(using_parser,sock_data, current_character);
                break;
            case READ_IPV6_ATYP_EVENT:
                handle_ipv6_atyp_read_event(using_parser,sock_data, current_character);
                break;
            case READ_ADDR_ATYP_EVENT:
                handle_addr_atyp_read_event(using_parser,sock_data, current_character);
                break;
            case IPV4_READING_EVENT:
                handle_ipv4_reading_event(using_parser,sock_data, current_character);
                break;
            case IPV6_READING_EVENT:
                handle_ipv6_reading_event(using_parser,sock_data, current_character);
                break;
            case ADDR_LEN_READING_EVENT:
                handle_addrlen_read_event(sock_data, current_character);
                break;
            case ADDR_READING_EVENT:
                handle_addr_reading_event(using_parser,sock_data, current_character);
                break;
            case PORT_READING_EVENT:
                handle_port_reading_event(using_parser,sock_data, current_character);
                break;
        }
        if(current_event->type == ERROR_FOUND_EVENT){
            printf("Invalid input");
            break;
        }
    }
    if(using_parser->state == END)
        return true;
    return false;
}

void close_sock_request_parser(struct parser * using_parser){
    if(using_parser != NULL)
        parser_destroy(using_parser);
}


void  close_sock_request_message(struct sock_request_message * message){
    free(message->port);
    if(message->atyp == 1)
        free(message->ipv4);
    else if(message->atyp == 4  )
        free(message->ipv6);
    else free(message->address);
    free(message);
}

