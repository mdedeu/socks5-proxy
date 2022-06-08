#include "sock_hello_parser.h"


enum states_and_events{
    INITIAL_STATE,
    VERSION_READ,
    NMETHODS_READ,
    READING_METHODS,
    END,

    VERSION_READ_EVENT,
    NMETHODS_READ_EVENT,
    METHODS_READ_EVENT,
    END_REACH_EVENT,
    ERROR_FOUND_EVENT
};


void hello_read_version(struct  parser_event * event , uint8_t c){
    event->type = VERSION_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
void read_nmethods(struct parser_event * event , uint8_t c){
    event->type = NMETHODS_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
void read_method(struct parser_event * event , uint8_t c){
    event->type = METHODS_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void handle_version_read_event(struct sock_hello_message * current_data,uint8_t version ){
    current_data->version = version;
}

static void handle_nmethods_read_event(struct sock_hello_message * current_data,uint8_t nmethods ){
    current_data->nmethods = nmethods;
    current_data->methods= malloc(nmethods);
}

static void handle_method_read_event(struct sock_hello_message * current_data , uint8_t method){
    //finish if reading username/authentication method
    current_data->methods[current_data->last_method_added] = method;
    current_data->last_method_added ++ ;
    if(current_data->last_method_added== current_data->nmethods){
        current_data->using_parser->state = END;
    }
}



static struct parser_state_transition initial_state_transitions[] ={
        {.when=ANY,.dest=VERSION_READ,.act1=hello_read_version}
};
static  struct parser_state_transition version_read_transitions[]={
        {.when=ANY,.dest=NMETHODS_READ,.act1=read_nmethods}
};
static struct parser_state_transition nmethod_read_transition[]={
        {.when=ANY,.dest=READING_METHODS,.act1=read_method}
};
static struct parser_state_transition reading_method_transitions[]={
        {.when=ANY,.dest=READING_METHODS,.act1=read_method}
};


static const struct parser_state_transition  * sock_hello_transitions[] = {initial_state_transitions,version_read_transitions,nmethod_read_transition,reading_method_transitions};

static const size_t state_transition_count[] = {
        N(initial_state_transitions),
        N(version_read_transitions),
        N(nmethod_read_transition),
        N(reading_method_transitions)
};


static struct parser_definition sock_parser_definition={
        .start_state=INITIAL_STATE,
        .states=sock_hello_transitions,
        .states_count=N(sock_hello_transitions),
        .states_n=state_transition_count
};


struct sock_hello_message * init_sock_hello_parser(){
    struct sock_hello_message * new_sock_hello_message = malloc(sizeof (struct sock_hello_message));
    struct parser * sock_hello_parser = parser_init(parser_no_classes(),&sock_parser_definition);
    new_sock_hello_message->using_parser = sock_hello_parser;
    new_sock_hello_message->last_method_added=0;
    return new_sock_hello_message;
}

bool feed_sock_hello_parser(struct sock_hello_message * sock_data ,char * input,int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (sock_data->using_parser->state != END  ); i++){
        current_event = parser_feed(sock_data->using_parser,input[i]);
        uint8_t  current_character = current_event->data[0];
        switch (current_event->type) {
            case VERSION_READ_EVENT:
                handle_version_read_event(sock_data,current_character);
                break;
            case NMETHODS_READ_EVENT:
                handle_nmethods_read_event(sock_data,current_character);
                break;
            case METHODS_READ_EVENT:
                handle_method_read_event(sock_data, current_character);
                break;
            case END :
//                end_parser_handler(sock_data,current_character);
                break;
            case ERROR_FOUND_EVENT: printf("ERROR_FOUND_EVENT");break;
        }
        if(current_event->type ==ERROR_FOUND_EVENT)
            break;
    }
    if(sock_data->using_parser->state == END)
        return true;
    return false;
}

void close_sock_hello_parser(struct sock_hello_message *  current_data){
    parser_destroy(current_data->using_parser);
    free(current_data->methods);
    free(current_data);
}


