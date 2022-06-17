#include "generalResponseParser.h"

enum states_and_events{
    INITIAL_STATE,
    ACTION_READ,
    METHOD_READ,
    READING_RLEN,
    READING_RESPONSE,
    END,

    ACTION_READ_EVENT,
    METHOD_READ_EVENT,
    READING_RLEN_EVENT,
    READING_RESPONSE_EVENT,
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
    event->type = READING_RLEN_EVENT;
    event->data[0]=c;
    event->n=1;
}
void save_response_character(struct parser_event * event, uint8_t c){
    event->type = READING_RESPONSE_EVENT;
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
    current_data->response_length += response_length << (8 * ( 1 - current_data->response_length_characaters_read++));
    if(current_data->response_length_characaters_read == 2){
        if(!current_data->response_length){
            current_data->using_parser->state = END;
            return;
        }
        current_data->response = malloc(current_data->response_length + 1);
        current_data->response[current_data->response_length] = 0;
        current_data->using_parser->state = READING_RESPONSE;
    }
}

static void handle_response_read_event(struct general_response_message * current_data , uint8_t response_character){
    current_data->response[current_data->response_characters_read] = response_character;
    current_data->response_characters_read++;
    if(current_data->response_characters_read == current_data->response_length){
        current_data->response[current_data->response_characters_read]=0;
        current_data->using_parser->state = END;
    }
}

static struct parser_state_transition initial_state_transitions[] ={
        {.when=ANY,.dest=ACTION_READ,.act1=check_action}
};
static struct parser_state_transition action_read_transitions[] ={
        {.when=ANY,.dest=METHOD_READ,.act1=check_method}
};
static  struct parser_state_transition method_read_transitions[]={
        {.when=ANY,.dest=READING_RLEN,.act1=save_response_length}
};
static struct parser_state_transition reading_rlen_transitions[]={
        {.when=ANY,.dest=READING_RLEN,.act1=save_response_length}
};
static struct parser_state_transition reading_response_transitions[]={
        {.when=ANY,.dest=READING_RESPONSE,.act1=save_response_character}
};

static const struct parser_state_transition * general_response_transitions[] = {
    initial_state_transitions,
    action_read_transitions,
    method_read_transitions,
    reading_rlen_transitions,
    reading_response_transitions
};

static const size_t state_transition_count[] = {
        N(initial_state_transitions),
        N(action_read_transitions),
        N(method_read_transitions),
        N(reading_rlen_transitions),
        N(reading_response_transitions)
};

static struct parser_definition general_response_parser_definition={
        .start_state=INITIAL_STATE,
        .states=general_response_transitions,
        .states_count=N(general_response_transitions),
        .states_n=state_transition_count
};

struct general_response_message * init_general_response_parser(){
    struct general_response_message * new_general_response_message = malloc(sizeof (struct general_response_message));
    memset(new_general_response_message, 0, sizeof(struct general_response_message));
    struct parser * general_response_parser = parser_init(parser_no_classes(),&general_response_parser_definition);
    new_general_response_message->using_parser = general_response_parser;
    //new_general_response_message->response_length=0;
    //new_general_response_message->response_characters_read=0;
    return new_general_response_message;
}

bool feed_general_response_parser(struct general_response_message * response_data, unsigned char * input, int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (response_data->using_parser->state != END  ); i++){
        current_event = parser_feed(response_data->using_parser,input[i]);
        uint8_t current_character = current_event->data[0];
        switch (current_event->type) {
            case ACTION_READ_EVENT:
                handle_action_read_event(response_data,current_character);
                break;
            case METHOD_READ_EVENT:
                handle_method_read_event(response_data,current_character);
                break;
            case READING_RLEN_EVENT:
                handle_rlen_read_event(response_data,current_character);
                break;
            case READING_RESPONSE_EVENT:
                handle_response_read_event(response_data, current_character);
                break;
            case END :
//                end_parser_handler(response_data,current_character);
                break;
            case ERROR_FOUND_EVENT: printf("ERROR_FOUND_EVENT");break;
        }
        if(current_event->type == ERROR_FOUND_EVENT)
            break;
    }

    if((response_data->using_parser->state != END  ))
        return false;
    else return true;
}

void close_general_response_parser(struct general_response_message * current_data){
    if(current_data == NULL)
        return;

    if(current_data->using_parser != NULL)
        parser_destroy(current_data->using_parser);
    if(current_data->response != NULL)
        free(current_data->response);

    free(current_data);
}

