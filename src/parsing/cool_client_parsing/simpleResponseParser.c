#include "simpleResponseParser.h"

enum states_and_events{
    INITIAL_STATE,
    STATUS_READ,
    END,

    STATUS_READ_EVENT,
    END_REACH_EVENT,
    ERROR_FOUND_EVENT
};

static void check_status(struct parser_event * event, uint8_t c){
    event->type = STATUS_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void handle_status_read_event(struct simple_response_message * current_data, uint8_t status){
    current_data->status[current_data->status_bytes_read++] = status;
    if(current_data->status_bytes_read == 2)
        current_data->using_parser->state = END;
}

static struct parser_state_transition initial_state_transitions[] ={
    {.when=ANY,.dest=STATUS_READ,.act1=check_status}
};

static struct parser_state_transition status_read_transitions[] ={
        {.when=ANY,.dest=STATUS_READ,.act1=check_status}
};

static const struct parser_state_transition * simple_response_transitions[] = {
    initial_state_transitions,
    status_read_transitions
};

static const size_t state_transition_count[] = {
        N(initial_state_transitions),
        N(status_read_transitions),
};

static struct parser_definition simple_response_parser_definition={
        .start_state=INITIAL_STATE,
        .states=simple_response_transitions,
        .states_count=N(simple_response_transitions),
        .states_n=state_transition_count
};

struct simple_response_message * init_simple_response_parser(){
    struct simple_response_message * new_simple_response_message = malloc(sizeof (struct simple_response_message));
    memset(new_simple_response_message, 0, sizeof(struct simple_response_message));
    struct parser * simple_response_parser = parser_init(parser_no_classes(), &simple_response_parser_definition);
    new_simple_response_message->using_parser = simple_response_parser;
    //new_simple_response_message->status_bytes_read = 0;
    return new_simple_response_message;
}

bool feed_simple_response_parser(struct simple_response_message * response_data, unsigned char * input, int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (response_data->using_parser->state != END  ); i++){
        current_event = parser_feed(response_data->using_parser,input[i]);
        uint8_t current_character = current_event->data[0];
        switch (current_event->type) {
            case STATUS_READ_EVENT:
                handle_status_read_event(response_data,current_character);
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

void close_simple_response_parser(struct simple_response_message * current_data){
    if(current_data == NULL)
        return;
    if(current_data->using_parser != NULL)
        parser_destroy(current_data->using_parser);

    free(current_data);
}

