#include "cool_request_parser.h"

enum states_and_events{
    INITIAL_STATE,

    QUERY_ACTION_READ,


    MODIFY_ACTION_READ,


    ADDING_USERNAME_METHOD_READ,
    ADDING_USERNAME_ULEN_READ,
    ADDING_USERNAME_USERNAME_READING,
    ADDING_USERNAME_USERNAME_READ,
    ADDING_USERNAME_PLEN_READ,
    ADDING_USERNAME_PASSWORD_READING,


    REMOVING_USERNAME_METHOD_READ,
    REMOVING_USERNAME_ULEN_READ,
    REMOVING_USERNAME_USERNAME_READING,


    PASSWORD_METHOD_READ,

    BUFFER_SIZE_METHOD_READ,
    BUFFER_SIZE_READING,

    PASSWORD_PROTOCOL_READ,
    QUERY_METHOD_READ,


    QUERY_ACTION_READ_EVENT,
    QUERY_METHOD_READ_EVENT,


    MODIFY_ACTION_READ_EVENT,
    ADDING_USERNAME_METHOD_READ_EVENT,
    REMOVING_USERNAME_METHOD_READ_EVENT,
    PASSWORD_METHOD_READ_EVENT,
  
    ADDING_ULEN_READ_EVENT,
    REMOVING_ULEN_READ_EVENT,
    PASSWORD_PROTOCOL_READ_EVENT,
    ADDING_USERNAME_USERNAME_READING_EVENT,
    REMOVING_USERNAME_USERNAME_READING_EVENT,
    ADDING_USERNAME_PLEN_READ_EVENT,
    ADDING_USERNAME_PASSWORD_READING_EVENT,
    BUFFER_SIZE_METHOD_READ_EVENT,
    BUFFER_SIZE_READING_EVENT,

    END

};



static void initial_state(struct  parser_event * event , uint8_t c){
    if(c == QUERY)
        event->type = QUERY_ACTION_READ_EVENT;
    else event->type = MODIFY_ACTION_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void query_action_read(struct  parser_event * event , uint8_t c){
    event->type = QUERY_METHOD_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}


static void modify_action_read(struct  parser_event * event , uint8_t c){
    if( c == 0 ) event->type = ADDING_USERNAME_METHOD_READ_EVENT;
    else if (c == 1 ) event->type = REMOVING_USERNAME_METHOD_READ_EVENT; 
    else if (c == 2) event->type = PASSWORD_METHOD_READ_EVENT;
    else if (c == 3) event->type = PASSWORD_METHOD_READ_EVENT;
    else event->type = BUFFER_SIZE_METHOD_READ_EVENT;
    event->data[0]=c;
    event->n=1;
}

static void adding_ulen_read(struct  parser_event * event , uint8_t c){
    event->type = ADDING_ULEN_READ_EVENT; 
    event->data[0]=c;
    event->n=1;
}


static void removing_ulen_read(struct  parser_event * event , uint8_t c){
    event->type = REMOVING_ULEN_READ_EVENT; 
    event->data[0]=c;
    event->n=1;
}
static void password_protocol_read(struct  parser_event * event , uint8_t c){
    event->type = PASSWORD_PROTOCOL_READ_EVENT; 
    event->data[0]=c;
    event->n=1;
}


static void adding_username_read(struct  parser_event * event , uint8_t c){
    event->type = ADDING_USERNAME_USERNAME_READING_EVENT; 
    event->data[0]=c;
    event->n=1;
}
static void removing_username_read(struct  parser_event * event , uint8_t c){
    event->type = REMOVING_USERNAME_USERNAME_READING_EVENT; 
    event->data[0]=c;
    event->n=1;
}

static void adding_plen_read(struct  parser_event * event , uint8_t c){
    event->type = ADDING_USERNAME_PLEN_READ_EVENT; 
    event->data[0]=c;
    event->n=1;
}

static void adding_password_reading(struct  parser_event * event, uint8_t c){
    event->type = ADDING_USERNAME_PASSWORD_READING_EVENT; 
    event->data[0]=c;
    event->n=1;
}

static void buffer_size_reading(struct  parser_event * event, uint8_t c){
    event->type = BUFFER_SIZE_READING_EVENT; 
    event->data[0]=c;
    event->n=1;
}


static struct parser_state_transition initial_state_transitions[] ={
        {.when=QUERY,.dest=QUERY_ACTION_READ,.act1=initial_state},
        {.when=MODIFY,.dest=MODIFY_ACTION_READ,.act1=initial_state},
        //{.when=ANY,.}
};

static struct parser_state_transition query_action_read_transitions[] ={
        {.when=ANY,.dest=QUERY_METHOD_READ,.act1=query_action_read},
};

static struct parser_state_transition modify_action_read_transitions[] ={
        {.when=0,.dest=ADDING_USERNAME_METHOD_READ,.act1=modify_action_read},
        {.when=1,.dest=REMOVING_USERNAME_METHOD_READ,.act1=modify_action_read},
        {.when=2,.dest=PASSWORD_METHOD_READ,.act1=modify_action_read},
        {.when=3,.dest=PASSWORD_METHOD_READ,.act1=modify_action_read},
        {.when=4,.dest=BUFFER_SIZE_METHOD_READ,.act1=modify_action_read}
};

static struct parser_state_transition adding_username_method_read_transitions[] ={
        {.when=ANY,.dest=ADDING_USERNAME_ULEN_READ,.act1=adding_ulen_read},
};
static struct parser_state_transition removing_username_method_read_transitions[] ={
        {.when=ANY,.dest=REMOVING_USERNAME_ULEN_READ,.act1=removing_ulen_read},
};
static struct parser_state_transition password_method_read_transitions[] ={
        {.when=ANY,.dest=PASSWORD_PROTOCOL_READ,.act1=password_protocol_read} //end 
};

static struct parser_state_transition adding_username_ulen_read_transitions[] ={
        {.when=ANY,.dest=ADDING_USERNAME_USERNAME_READING,.act1=adding_username_read}
};
static struct parser_state_transition removing_username_ulen_read_transitions[] ={
        {.when=ANY,.dest=REMOVING_USERNAME_USERNAME_READING,.act1=removing_username_read}
};

static struct parser_state_transition adding_username_username_reading_transitions[] ={
        {.when=ANY,.dest=ADDING_USERNAME_USERNAME_READING,.act1=adding_username_read}
};
static struct parser_state_transition removing_username_username_reading_transitions[] ={
        {.when=ANY,.dest=REMOVING_USERNAME_USERNAME_READING,.act1=removing_username_read} //end
};

static struct parser_state_transition adding_username_username_read_transitions[] ={
        {.when=ANY,.dest=ADDING_USERNAME_PLEN_READ,.act1=adding_plen_read}
};

static struct parser_state_transition adding_username_plen_read_transitions[] ={
        {.when=ANY,.dest=ADDING_USERNAME_PASSWORD_READING,.act1=adding_password_reading} //end
};

static struct parser_state_transition adding_username_password_reading_transitions[] ={
        {.when=ANY,.dest=ADDING_USERNAME_PASSWORD_READING,.act1=adding_password_reading} //end
};

static struct parser_state_transition buffer_size_method_read_transitions[] ={
        {.when=ANY,.dest=BUFFER_SIZE_READING,.act1=buffer_size_reading} //end 
};
static struct parser_state_transition buffer_size_reading_transitions[] ={
        {.when=ANY,.dest=BUFFER_SIZE_READING,.act1=buffer_size_reading} //end
};


static const struct parser_state_transition  * general_parser_transitions[] = {
        initial_state_transitions,
        query_action_read_transitions,
        modify_action_read_transitions,
        adding_username_method_read_transitions,
        adding_username_ulen_read_transitions,
        adding_username_username_reading_transitions,
        adding_username_username_read_transitions,
        adding_username_plen_read_transitions,
        adding_username_password_reading_transitions,
        removing_username_method_read_transitions,
        removing_username_ulen_read_transitions,
        removing_username_username_reading_transitions,
        password_method_read_transitions,
        buffer_size_method_read_transitions,
        buffer_size_reading_transitions
};

static const size_t  general_parser_transitions_count[] = {
        N(initial_state_transitions),
        N(query_action_read_transitions),
        N(modify_action_read_transitions),
        N(adding_username_method_read_transitions),
        N(adding_username_ulen_read_transitions),
        N(adding_username_username_reading_transitions),
        N(adding_username_username_read_transitions),
        N(adding_username_plen_read_transitions) ,
        N(adding_username_password_reading_transitions),
        N(removing_username_method_read_transitions),
        N(removing_username_ulen_read_transitions),
        N(removing_username_username_reading_transitions),
        N(password_method_read_transitions),
        N(buffer_size_method_read_transitions),
        N(buffer_size_reading_transitions)
};


static struct parser_definition general_parser_definition={
        .start_state=INITIAL_STATE,
        .states=general_parser_transitions,
        .states_count=N(general_parser_transitions),
        .states_n=general_parser_transitions_count
};


// handles query and modify action event 
void handle_action_read_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->action = current_character;

}

void handle_query_method_read_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->method = current_character;
    general_request_data->using_parser->state = END;
}

//handles adding , removing and password  
void handle_modify_method_read_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->method = current_character;

}

void handle_password_protocol_read_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->protocol = current_character;
    general_request_data->using_parser->state = END;
}

void handle_ulen_read_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->ulen = current_character;
    general_request_data->username = malloc(current_character + 1 );
}

void handle_adding_username_username_reading_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->username[general_request_data->username_characters_read] = current_character;
    general_request_data->username_characters_read++;
if(general_request_data->username_characters_read == general_request_data->ulen){
    general_request_data->username[general_request_data->username_characters_read] = 0;
    general_request_data->using_parser->state = ADDING_USERNAME_USERNAME_READ;
}
}

void handle_removing_username_reading_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->username[general_request_data->username_characters_read] = current_character;
    general_request_data->username_characters_read++;
if(general_request_data->username_characters_read == general_request_data->ulen){
    general_request_data->username[general_request_data->username_characters_read] = 0;
    general_request_data->using_parser->state= END;
}
}

void handle_adding_username_plen_read_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->password = malloc(current_character + 1 );
    general_request_data->plen = current_character;
}

void handle_adding_username_password_reading_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->password[general_request_data->password_characters_read] = current_character;
    general_request_data->password_characters_read++;
if(general_request_data->password_characters_read == general_request_data->plen){
    general_request_data->password[general_request_data->password_characters_read] = 0;
    general_request_data->using_parser->state = END;
}
}

void handle_buffer_size_reading_event(struct general_request_message* general_request_data ,uint8_t current_character){
    general_request_data->buffer_size += current_character << (8 * ( 1 - general_request_data->buffer_size_bytes_read++));
    if(general_request_data->buffer_size_bytes_read == 2){
        general_request_data->using_parser->state = END;
    }
}

struct general_request_message * init_general_parser(){
    struct general_request_message * new_general_request_message = malloc(sizeof (struct general_request_message));
    memset(new_general_request_message, 0, sizeof(struct general_request_message));
    struct parser * using_parser = parser_init(parser_no_classes(),&general_parser_definition);
    new_general_request_message->using_parser = using_parser;
    //new_general_request_message->username_characters_read=0;
    //new_general_request_message->password_characters_read=0;
    //new_general_request_message->plen = 0 ;
    //new_general_request_message->ulen = 0 ; 
    return new_general_request_message;
}

void destroy_general_request_parser(struct parser*  using_parser){
    if(using_parser != NULL)
        parser_destroy(using_parser);
}

void destroy_general_request_message(struct general_request_message * message){
    if(message == NULL)
        return;
    if(message->ulen != 0 )
        free(message->username);
    if(message->plen != 0 )
    free(message->password);               

    free(message); 
}


bool feed_general_request_parser(struct general_request_message * general_request_data ,char * input,int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (general_request_data->using_parser->state != END  ); i++){
        current_event = parser_feed(general_request_data->using_parser,input[i]);
        uint8_t  current_character = current_event->data[0];
        switch (current_event->type) {
            case QUERY_ACTION_READ_EVENT:
                handle_action_read_event(general_request_data,current_character);
                break;
            case QUERY_METHOD_READ_EVENT:
                handle_query_method_read_event(general_request_data,current_character);
                break;
            case MODIFY_ACTION_READ_EVENT:
                handle_action_read_event(general_request_data,current_character);
                break;
            case ADDING_USERNAME_METHOD_READ_EVENT:
                handle_modify_method_read_event(general_request_data,current_character);
                break;
            case REMOVING_USERNAME_METHOD_READ_EVENT:
                handle_modify_method_read_event(general_request_data,current_character);
                break;
            case PASSWORD_METHOD_READ_EVENT:
                handle_modify_method_read_event(general_request_data,current_character);
                break;
            case BUFFER_SIZE_METHOD_READ_EVENT:
                handle_modify_method_read_event(general_request_data,current_character);
                break;
            case ADDING_ULEN_READ_EVENT:
                handle_ulen_read_event(general_request_data, current_character);
                break;
            case REMOVING_ULEN_READ_EVENT:
                handle_ulen_read_event(general_request_data,current_character);
                break;
            case PASSWORD_PROTOCOL_READ_EVENT:
                handle_password_protocol_read_event(general_request_data,current_character);
                break;
            case ADDING_USERNAME_USERNAME_READING_EVENT:
                handle_adding_username_username_reading_event(general_request_data,current_character);
                break;
            case REMOVING_USERNAME_USERNAME_READING_EVENT:
                handle_removing_username_reading_event(general_request_data,current_character);
                break;
            case ADDING_USERNAME_PLEN_READ_EVENT:
                handle_adding_username_plen_read_event(general_request_data,current_character);
                break;
            case ADDING_USERNAME_PASSWORD_READING_EVENT:
                handle_adding_username_password_reading_event(general_request_data,current_character);
                break;
            case BUFFER_SIZE_READING_EVENT:
                handle_buffer_size_reading_event(general_request_data,current_character);
                break;
            case END :
//                end_parser_handler(general_request_data,current_character);
                break;
//            case ERROR_FOUND_EVENT: printf("ERROR_FOUND_EVENT");break;
        }
//        if(current_event->type ==ERROR_FOUND_EVENT)
//            break;
        if(general_request_data->using_parser->state == END)
            return true;
    }
    return false;
}