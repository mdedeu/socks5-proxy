#include "pop3_authentication_parser.h"

#define POP3_CHECK "+OK POP"
#define OK "+OK"
#define ERR "-ERR"

enum states_and_events{
    INITIAL_STATE,
    CHECKING_POP3,
    READING_USERNAME,
    CHECKING_USERNAME,
    READING_PASSWORD,
    CHECKING_PASSWORD,
    END,

    POP3_CHECK_EVENT,
    USERNAME_READ_EVENT,
    USERNAME_CHECK_EVENT,
    PASSWORD_READ_EVENT,
    PASSWORD_CHECK_EVENT,
    END_REACH_EVENT,
    ERROR_FOUND_EVENT
};

void save_pop3_check_character(struct parser_event * event , uint8_t c){
    event->type = POP3_CHECK_EVENT;
    event->data[0]=c;
    event->n=1;
}
void save_username_character(struct parser_event * event , uint8_t c){
    event->type = USERNAME_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
void save_username_check_character(struct parser_event * event , uint8_t c){
    event->type = USERNAME_CHECK_EVENT;
    event->data[0]=c;
    event->n=1;
}
void save_password_character(struct parser_event * event , uint8_t c){
    event->type = PASSWORD_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
void save_password_check_character(struct parser_event * event , uint8_t c){
    event->type = PASSWORD_CHECK_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void handle_pop3_check_event(struct pop3_authentication_message * current_data,uint8_t version ){
}
static void handle_username_read_event(struct pop3_authentication_message * current_data,uint8_t version ){
}
static void handle_username_check_event(struct pop3_authentication_message * current_data,uint8_t version ){
}
static void handle_password_read_event(struct pop3_authentication_message * current_data,uint8_t version ){
}
static void handle_password_check_event(struct pop3_authentication_message * current_data,uint8_t version ){
}

static struct parser_state_transition initial_state_transitions[] ={
        {.when=ANY,.dest=CHECKING_POP3,.act1=save_pop3_check_character}
};
static struct parser_state_transition checking_pop3_transitions[] ={
        {.when=ANY,.dest=CHECKING_POP3,.act1=save_pop3_check_character}
};
static struct parser_state_transition checked_pop3_transitions[] ={
        {.when=ANY,.dest=READING_USERNAME,.act1=save_username_character}
};
static struct parser_state_transition reading_username_transitions[] ={
        {.when=ANY,.dest=READING_USERNAME,.act1=save_username_character}
};
static struct parser_state_transition read_username_transitions[] ={
        {.when=ANY,.dest=CHECKING_USERNAME,.act1=save_username_check_character}
};
static struct parser_state_transition checking_username_transitions[] ={
        {.when=ANY,.dest=CHECKING_USERNAME,.act1=save_username_check_character}
};
static struct parser_state_transition checked_username_transitions[] ={
        {.when=ANY,.dest=READING_PASSWORD,.act1=save_password_character}
};
static struct parser_state_transition reading_password_transitions[] ={
        {.when=ANY,.dest=READING_PASSWORD,.act1=save_password_character}
};
static struct parser_state_transition read_password_transitions[] ={
        {.when=ANY,.dest=CHECKING_PASSWORD,.act1=save_password_check_character}
};
static struct parser_state_transition checking_password_transitions[] ={
        {.when=ANY,.dest=CHECKING_PASSWORD,.act1=save_password_check_character}
};

static const struct parser_state_transition  * pop3_authentication_transitions[] = {
    initial_state_transitions,
    checking_pop3_transitions,
    checked_pop3_transitions,
    reading_username_transitions,
    read_username_transitions,
    checking_username_transitions,
    checked_username_transitions,
    reading_password_transitions,
    read_password_transitions,
    checking_password_transitions
};

static const size_t state_transition_count[] = {
        N(initial_state_transitions),
        N(checking_pop3_transitions),
        N(checked_pop3_transitions),
        N(reading_username_transitions),
        N(read_username_transitions),
        N(checking_username_transitions),
        N(checked_username_transitions),
        N(reading_password_transitions),
        N(read_password_transitions),
        N(checking_password_transitions)
};


static struct parser_definition pop3_parser_definition={
        .start_state=INITIAL_STATE,
        .states=pop3_authentication_transitions,
        .states_count=N(pop3_authentication_transitions),
        .states_n=state_transition_count
};


struct pop3_authentication_message * init_pop3_authentication_parser(){
    struct pop3_authentication_message * new_pop3_authentication_message = malloc(sizeof (struct pop3_authentication_message));
    struct parser * pop3_authentication_parser = parser_init(parser_no_classes(),&pop3_parser_definition);
    new_pop3_authentication_message->using_parser = pop3_authentication_parser;
    new_pop3_authentication_message->check_characters_read=0;
    new_pop3_authentication_message->password_characters_read=0;
    new_pop3_authentication_message->username_characters_read=0;
    return new_pop3_authentication_message;
}

bool feed_pop3_authentication_parser(struct pop3_authentication_message * pop3_data ,char * input,int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (pop3_data->using_parser->state != END  ); i++){
        current_event = parser_feed(pop3_data->using_parser,input[i]);
        uint8_t  current_character = current_event->data[0];
        switch (current_event->type) {
            case VERSION_READ_EVENT:
                handle_version_read_event(pop3_data,current_character);
                break;
            case ULEN_READ_EVENT:
                handle_ulen_read_event(pop3_data,current_character);
                break;
            case USERNAME_READ_EVENT:
                handle_username_read_event(pop3_data, current_character);
                break;
            case PLEN_READ_EVENT:
                handle_plen_read_event(pop3_data,current_character);
                break;
            case READING_PASSWORD_EVENT:
                handle_password_read_event(pop3_data,current_character);
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

void close_pop3_authentication_parser(struct pop3_authentication_message * current_data){
    parser_destroy(current_data->using_parser);
    free(current_data);
}

