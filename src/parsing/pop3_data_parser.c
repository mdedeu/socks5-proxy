#include "pop3_data_parser.h"

enum states_and_events{
    INITIAL_STATE,
    CHECKING_POP3,
    READING_DATA,
    CLOSING,
    END,

    POP3_CHECK_EVENT,
    END_REACH_EVENT,
    DATA_READ_EVENT,
    CLOSE_EVENT,
    ERROR_FOUND_EVENT
};

void save_pop3_check_character(struct parser_event * event , uint8_t c){
    event->type = POP3_CHECK_EVENT;
    event->data[0]=c;
    event->n=1;
}
void save_data_character(struct parser_event * event , uint8_t c){
    event->type = DATA_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
void save_closing_character(struct parser_event * event , uint8_t c){
    event->type = CLOSE_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void handle_pop3_check_event(struct pop3_data_message * current_data, uint8_t c){
    if(current_data->check_characters_read < current_data->prefix_len){
        if(c != current_data->prefix[current_data->check_characters_read]){
            current_data->connected = false;
            current_data->using_parser->state = END;
            return;
        }
        current_data->check_characters_read++;
        return;
    } 
    if(c == ' ')
        current_data->using_parser->state = READING_DATA;
}

static void handle_data_read_event(struct pop3_data_message * current_data, uint8_t c){
    if(current_data->data_characters_read >= BUFFER_SIZE-2){
        //TODO: mirar si aca habria que tirar un error o algo
        current_data->using_parser->state = END;
        return;
    }
    current_data->data[current_data->data_characters_read++] = c;
}

static void handle_close_event(struct pop3_data_message * current_data, uint8_t c){
    if(c != '\n'){
        current_data->data[current_data->data_characters_read++] = c;
        current_data->using_parser->state = READING_DATA;
    }
}


static struct parser_state_transition initial_state_transitions[] ={
        {.when=ANY,.dest=CHECKING_POP3,.act1=save_pop3_check_character}
};
static struct parser_state_transition checking_pop3_transitions[] ={
        {.when=ANY,.dest=CHECKING_POP3,.act1=save_pop3_check_character}
};
static struct parser_state_transition reading_data_transitions[] ={
        {.when='\r',.dest=CLOSING,.act1=save_closing_character},
        {.when=ANY,.dest=READING_DATA,.act1=save_data_character}
};
static struct parser_state_transition closing_transitions[] ={
    //TODO: mirar si al matchear con el \n se deja de tener en cuenta la transicion anterior por el orden
        {.when='\n',.dest=END,.act1=save_closing_character},
        {.when=ANY,.dest=CLOSING,.act1=save_closing_character}
};

static const struct parser_state_transition  * pop3_data_transitions[] = {
    initial_state_transitions,
    checking_pop3_transitions,
    reading_data_transitions,
    closing_transitions
};

static const size_t state_transition_count[] = {
        N(initial_state_transitions),
        N(checking_pop3_transitions),
        N(reading_data_transitions),
        N(closing_transitions)
};


static struct parser_definition pop3_parser_definition={
        .start_state=INITIAL_STATE,
        .states=pop3_data_transitions,
        .states_count=N(pop3_data_transitions),
        .states_n=state_transition_count
};


struct pop3_data_message * init_pop3_data_parser(){
    struct pop3_data_message * new_pop3_data_message = malloc(sizeof (struct pop3_data_message));
    struct parser * pop3_data_parser = parser_init(parser_no_classes(),&pop3_parser_definition);
    new_pop3_data_message->using_parser = pop3_data_parser;
    new_pop3_data_message->data_characters_read = 0;
    new_pop3_data_message->connected = true;
    return new_pop3_data_message;
}

bool feed_pop3_data_parser(struct pop3_data_message * pop3_data ,char * input,int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (pop3_data->using_parser->state != END  ); i++){
        current_event = parser_feed(pop3_data->using_parser,input[i]);
        uint8_t  current_character = current_event->data[0];
        switch (current_event->type) {
            case POP3_CHECK_EVENT:
                handle_pop3_check_event(pop3_data,current_character);
                break;
            case DATA_READ_EVENT:
                handle_data_read_event(pop3_data,current_character);
                break;
            case CLOSE_EVENT:
                handle_close_event(pop3_data,current_character);
                break;
            case END :
//                end_parser_handler(pop3_data,current_character);
                break;
            case ERROR_FOUND_EVENT: printf("ERROR_FOUND_EVENT");break;
        }
        if(current_event->type == ERROR_FOUND_EVENT)
            break;
    }

    if((pop3_data->using_parser->state != END  ))
        return false;
    else return true;
}

void close_pop3_data_parser(struct pop3_data_message * current_data){
    parser_destroy(current_data->using_parser);
    free(current_data);
}

