#include "generalResponseParser.h"

enum states_and_events{
    INITIAL_STATE,
    ACTION_READ,
    METHOD_READ,
    RLEN_READ,
    READING_RESPONSE,
    END,

    ACTION_READ_EVENT,
    METHOD_READ_EVENT,
    RLEN_READ_EVENT,
    RESPONSE_READ_EVENT,
    END_REACH_EVENT,
    ERROR_FOUND_EVENT
};

void check_action(struct parser_event * event, uint8_t c){
    event->type = ACTION_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
void check_method(struct parser_event * event, uint8_t c){
    event->type = METHOD_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
void save_response_length(struct parser_event * event, uint8_t c){
    event->type = RLEN_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
void save_response_character(struct parser_event * event, uint8_t c){
    event->type = RESPONSE_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void handle_action_read_event(struct general_response_message* current_data, uint8_t action){
    current_data->action = action;
}

static void handle_method_read_event(struct general_response_message * current_data, uint8_t method){
    current_data->method = method;
}

static void handle_rlen_read_event(struct general_response_message * current_data, uint8_t response_length){
    current_data->response_length = response_length;
    current_data->response = malloc(current_data->response_length + 1);
}

static void handle_response_read_event(struct general_response_message * current_data, uint8_t response_character){
    current_data->response[current_data->response_characters_read] = response_character;
    current_data->response_characters_read ++ ;
    if(current_data->response_characters_read == current_data->response_length){
        current_data->response[current_data->response_characters_read]=0;
        current_data->using_parser->state = FINISH_USERNAME;
    }
}

static struct parser_state_transition initial_state_transitions[] ={
        {.when=ANY,.dest=ACTION_READ,.act1=check_action}
};
static struct parser_state_transition action_read_transitions[] ={
        {.when=ANY,.dest=METHOD_READ,.act1=check_method}
};
static  struct parser_state_transition method_read_transitions[]={
        {.when=ANY,.dest=RLEN_READ,.act1=save_response_length}
};
static struct parser_state_transition rlen_read_transitions[]={
        {.when=ANY,.dest=READING_RESPONSE,.act1=save_response_character}
};
static struct parser_state_transition reading_response_transitions[]={
        {.when=ANY,.dest=READING_RESPONSE,.act1=save_response_character}
};
static const struct parser_state_transition * general_response_transitions[] = {
    initial_state_transitions,
    action_read_transitions,
    method_read_transitions,
    rlen_read_transitions,
    reading_response_transitions
};

static const size_t state_transition_count[] = {
        N(initial_state_transitions),
        N(action_read_transitions),
        N(method_read_transitions),
        N(rlen_read_transitions),
        N(reading_response_transitions)
};


static struct parser_definition sock_parser_definition={
        .start_state=INITIAL_STATE,
        .states=general_response_transitions,
        .states_count=N(general_response_transitions),
        .states_n=state_transition_count
};


struct general_response_message * init_sock_authentication_parser(){
    struct general_response_message * new_general_response_message = malloc(sizeof (struct general_response_message));
    struct parser * general_response_message = parser_init(parser_no_classes(),&sock_parser_definition);
    new_general_response_message->using_parser = general_response_message;
    new_general_response_message->response_length=0;
    new_general_response_message->response_characters_read=0;
    return new_general_response_message;
}

bool feed_general_response_parser(struct general_response_message * sock_data, char * input, int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (sock_data->using_parser->state != END  ); i++){
        current_event = parser_feed(sock_data->using_parser,input[i]);
        uint8_t current_character = current_event->data[0];
        switch (current_event->type) {
            case ACTION_READ_EVENT:
                handle_version_read_event(sock_data,current_character);
                break;
            case METHOD_READ_EVENT:
                handle_version_read_event(sock_data,current_character);
                break;
            case RLEN_READ_EVENT:
                handle_ulen_read_event(sock_data,current_character);
                break;
            case RESPONSE_READ_EVENT:
                handle_username_read_event(sock_data, current_character);
                break;
            case END :
//                end_parser_handler(sock_data,current_character);
                break;
            case ERROR_FOUND_EVENT: printf("ERROR_FOUND_EVENT");break;
        }
        if(current_event->type == ERROR_FOUND_EVENT)
            break;
    }

    if((sock_data->using_parser->state != END  ))
        return false;
    else return true;
}

void close_general_response_parser(struct general_response_message *  current_data){
    parser_destroy(current_data->using_parser);
    free(current_data->response);
    free(current_data);
}

