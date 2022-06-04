#include "sock_authentication_parser.h"

enum states_and_events{
    INITIAL_STATE,
    VERSION_READ,
    ULEN_READ,
    READING_USERNAME,
    FINISH_USERNAME,
    PLEN_READ,
    READING_PASSWORD,
    END,

    VERSION_READ_EVENT,
    ULEN_READ_EVENT,
    USERNAME_READ_EVENT,
    PLEN_READ_EVENT,
    READING_PASSWORD_EVENT,
    END_REACH_EVENT,
    ERROR_FOUND_EVENT
};

 void check_version(struct  parser_event * event , uint8_t c){
    event->type = VERSION_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
  void save_username_length(struct parser_event * event , uint8_t c){
    event->type = ULEN_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
  void save_username_character(struct parser_event * event , uint8_t c){
    event->type = USERNAME_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
  void save_password_length(struct parser_event * event , uint8_t c){
    event->type = PLEN_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}
  void save_password_character(struct parser_event * event , uint8_t c){
    event->type = READING_PASSWORD_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void handle_version_read_event(struct sock_authentication_message * current_data,uint8_t version ){
    current_data->version = version;
}

static void handle_ulen_read_event(struct sock_authentication_message * current_data,uint8_t username_length ){
    current_data->username_length = username_length;
    current_data->username= malloc(current_data->username_length + 1 );
}

static void handle_username_read_event(struct sock_authentication_message * current_data , uint8_t username_character){
    current_data->username[current_data->username_characters_read] = username_character;
    current_data->username_characters_read ++ ;
    if(current_data->username_characters_read == current_data->username_length){
        current_data->username[current_data->username_characters_read]=0;
        current_data->using_parser->state = FINISH_USERNAME;
    }
}

static void handle_plen_read_event(struct sock_authentication_message * current_data,uint8_t password_length ){
    current_data->password_length = password_length;
    current_data->password= malloc(current_data->password_length + 1);
}

static void handle_password_read_event(struct sock_authentication_message * current_data , uint8_t password_character){
    current_data->password[current_data->password_characters_read] = password_character;
    current_data->password_characters_read ++ ;
    if(current_data->password_characters_read == current_data->password_length){
        current_data->password[current_data->password_characters_read]=0;
        current_data->using_parser->state = END;
    }
}


static struct parser_state_transition initial_state_transitions[] ={
        {.when=ANY,.dest=VERSION_READ,.act1=check_version}
};
static  struct parser_state_transition version_read_transitions[]={
        {.when=ANY,.dest=ULEN_READ,.act1=save_username_length}
};
static struct parser_state_transition ulen_read_transitions[]={
        {.when=ANY,.dest=READING_USERNAME,.act1=save_username_character}
};
static struct parser_state_transition reading_username_transitions[]={
        {.when=ANY,.dest=READING_USERNAME,.act1=save_username_character}
};
static struct parser_state_transition finish_username_transitions[]={
        {.when=ANY,.dest=PLEN_READ,.act1=save_password_length}
};
static struct parser_state_transition reading_password_length_transitions[]={
        {.when=ANY,.dest=READING_PASSWORD,.act1=save_password_character}
};
static struct parser_state_transition reading_password_transitions[]={
        {.when=ANY,.dest=READING_PASSWORD,.act1=save_password_character}
};
static const struct parser_state_transition  * sock_authentication_transitions[] = {initial_state_transitions,version_read_transitions,ulen_read_transitions,reading_username_transitions,
        finish_username_transitions,reading_password_length_transitions,reading_password_transitions};

static const size_t state_transition_count[] = {
        N(initial_state_transitions),
        N(version_read_transitions),
        N(ulen_read_transitions),
        N(reading_username_transitions),
        N(finish_username_transitions),
        N(reading_password_length_transitions),
        N(reading_password_transitions),
};


static struct parser_definition sock_parser_definition={
        .start_state=INITIAL_STATE,
        .states=sock_authentication_transitions,
        .states_count=N(sock_authentication_transitions),
        .states_n=state_transition_count
};


struct sock_authentication_message * init_sock_authentication_parser(){
    struct sock_authentication_message * new_sock_authentication_message = malloc(sizeof (struct sock_authentication_message));
    struct parser * sock_authentication_parser = parser_init(parser_no_classes(),&sock_parser_definition);
    new_sock_authentication_message->using_parser = sock_authentication_parser;
    new_sock_authentication_message->password_length=0;
    new_sock_authentication_message->password_characters_read=0;
    new_sock_authentication_message->username_characters_read=0;
    new_sock_authentication_message->username_length = 0;
    return new_sock_authentication_message;
}

 void feed_sock_authentication_parser(struct sock_authentication_message * sock_data ,char * input,int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (sock_data->using_parser->state != END  ); i++){
        current_event = parser_feed(sock_data->using_parser,input[i]);
        uint8_t  current_character = current_event->data[0];
        switch (current_event->type) {
            case VERSION_READ_EVENT:
                handle_version_read_event(sock_data,current_character);
                break;
            case ULEN_READ_EVENT:
                handle_ulen_read_event(sock_data,current_character);
                break;
            case USERNAME_READ_EVENT:
                handle_username_read_event(sock_data, current_character);
                break;
            case PLEN_READ_EVENT:
                handle_plen_read_event(sock_data,current_character);
                break;
            case READING_PASSWORD_EVENT:
                handle_password_read_event(sock_data,current_character);
                break;
            case END :
//                end_parser_handler(sock_data,current_character);
                break;
            case ERROR_FOUND_EVENT: printf("ERROR_FOUND_EVENT");break;
        }
        if(current_event->type ==ERROR_FOUND_EVENT)
            break;
    }
}

void close_sock_authentication_parser(struct sock_authentication_message *  current_data){
    parser_destroy(current_data->using_parser);
    free(current_data->username);
    free(current_data->password);
    free(current_data);
}

