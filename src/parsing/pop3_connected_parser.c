#include "pop3_connected_parser.h"

enum states_and_events{
    INITIAL_STATE,
    CHECKING_POP3,
    CLOSING,
    END,

    POP3_CHECK_EVENT,
    END_REACH_EVENT,
    CLOSE_EVENT,
    ERROR_FOUND_EVENT
};

static void save_pop3_check_character(struct parser_event * event , uint8_t c){
    event->type = POP3_CHECK_EVENT;
    event->data[0]=c;
    event->n=1;
}
static void save_closing_character(struct parser_event * event , uint8_t c){
    event->type = CLOSE_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void handle_pop3_check_event(struct pop3_connected_message * current_data, uint8_t c){
    if(current_data->check_characters_read < current_data->prefix_len){
        if(toupper(c) != toupper(current_data->prefix[current_data->check_characters_read])){
            current_data->connected = false;
            current_data->using_parser->state = END;
            return;
        }
        current_data->check_characters_read++;
    } 
}

static struct parser_state_transition initial_state_transitions[] ={
        {.when=ANY,.dest=CHECKING_POP3,.act1=save_pop3_check_character}
};
static struct parser_state_transition checking_pop3_transitions[] ={
        {.when='\r',.dest=CLOSING,.act1=save_closing_character},
        {.when=ANY,.dest=CHECKING_POP3,.act1=save_pop3_check_character}
};
static struct parser_state_transition closing_transitions[] ={
        {.when='\n',.dest=END,.act1=save_closing_character},
        {.when=ANY,.dest=CHECKING_POP3,.act1=save_pop3_check_character}
};

static const struct parser_state_transition  * pop3_connected_transitions[] = {
    initial_state_transitions,
    checking_pop3_transitions,
    closing_transitions
};

static const size_t state_transition_count[] = {
        N(initial_state_transitions),
        N(checking_pop3_transitions),
        N(closing_transitions)
};


static struct parser_definition pop3_parser_definition={
        .start_state=INITIAL_STATE,
        .states=pop3_connected_transitions,
        .states_count=N(pop3_connected_transitions),
        .states_n=state_transition_count
};


struct pop3_connected_message * init_pop3_connected_parser(){
    struct pop3_connected_message * new_pop3_connected_message = malloc(sizeof (struct pop3_connected_message));
    struct parser * pop3_connected_parser = parser_init(parser_no_classes(),&pop3_parser_definition);
    new_pop3_connected_message->using_parser = pop3_connected_parser;
    new_pop3_connected_message->check_characters_read=0;
    new_pop3_connected_message->connected = true;
    return new_pop3_connected_message;
}

bool feed_pop3_connected_parser(struct pop3_connected_message * pop3_data ,char * input,int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (pop3_data->using_parser->state != END  ); i++){
        current_event = parser_feed(pop3_data->using_parser,input[i]);
        uint8_t  current_character = current_event->data[0];
        switch (current_event->type) {
            case POP3_CHECK_EVENT:
                handle_pop3_check_event(pop3_data,current_character);
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

void close_pop3_connected_parser(struct pop3_connected_message * current_data){
    parser_destroy(current_data->using_parser);
    free(current_data);
}


//+OK POP me llamo salvador\r\n
//+OK POP me llamo salvador\r y mi apellido es castagnino\r\n
//+OK POP me llamo salvador\n y mi apellido es castagnino\r\n
//+OK PP me llamo salvador\r\n
//OK POP me llamo salvador\r\n
//OK OP me llamo salvador\r\n
//  int main(){
//       struct pop3_connected_message * testMessage = init_pop3_connected_parser();
//       testMessage->prefix_len = 7;
//       memcpy(testMessage->prefix,"+OK POP",8);
//       bool finished;

//       finished = feed_pop3_connected_parser(testMessage, "+OK m llam\rbbb\rbbb\n", 19);
//       printf("Connected:%d\n", (int) testMessage->connected);
//       printf("Finished:%d\n", (int) finished);
//   }
