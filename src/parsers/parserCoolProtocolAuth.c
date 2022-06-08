#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "../parser.h"

enum states{
	INITIAL_STATE,
	VERSION_READ,
	ULEN_READ,
	USERNAME_READING,
	FINISH_USERNAME,
	PASSLEN_READ,
	PASS_READING,
	END,
	ERROR,
};

enum event_types{
	VERSION_READ_EVENT,
	ULEN_READ_EVENT,
	USERNAME_READ_EVENT,
	PLEN_READ_EVENT,
	READING_PASSWORD_EVENT,
	END_EVENT,
	ERROR_FOUND_EVENT
};
enum errors{
	WRONG_VERSION
};


//actions

void error(struct parser_event * event, uint8_t c){
	event->type = ERROR_FOUND_EVENT;
	event->data[0] = WRONG_VERSION;
	event->n=1;
}

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


//handlers

static void handle_version_read_event(struct cool_protocol_authentication_message * current_data,uint8_t version ){
    current_data->version = version;
}

static void handle_ulen_read_event(struct cool_protocol_authentication_message * current_data,uint8_t username_length ){
    current_data->username_length = username_length;
    current_data->username= malloc(current_data->username_length + 1 );
}

static void handle_username_read_event(struct cool_protocol_authentication_message * current_data , uint8_t username_character){
    current_data->username[current_data->username_characters_read] = username_character;
    current_data->username_characters_read ++ ;
    if(current_data->username_characters_read == current_data->username_length){
        current_data->username[current_data->username_characters_read]=0;
        current_data->using_parser->state = FINISH_USERNAME;
    }
}

static void handle_plen_read_event(struct cool_protocol_authentication_message * current_data,uint8_t password_length ){
    current_data->password_length = password_length;
    current_data->password= malloc(current_data->password_length + 1);
}

static void handle_password_read_event(struct cool_protocol_authentication_message * current_data , uint8_t password_character){
    current_data->password[current_data->password_characters_read] = password_character;
    current_data->password_characters_read ++ ;
    if(current_data->password_characters_read == current_data->password_length){
        current_data->password[current_data->password_characters_read]=0;
        current_data->using_parser->state = END;
    }
}
static void handle_error_event(struct cool_protocol_authentication_message *  current_data, uint8_t error){

	switch(error){
		case WRONG_VERSION:
			printf("WRONG VERSION ERROR");break;
		default:
			printf("UNKNOWN ERROR"); break;
	}

}

//transitions & parser def.

static struct parser_state_transition initial_state_transitions[] ={
        {.when=ANY,.dest=VERSION_READ,.act1=check_version}
};
static  struct parser_state_transition version_read_transitions[]={
		{.when = 1, .dest=ULEN_READ, .act1=save_username_length}
        {.when=ANY,.dest=ERROR, .act1= error}
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
static const struct parser_state_transition  * cool_protocol_authentication_transitions[] = {initial_state_transitions,version_read_transitions,ulen_read_transitions,reading_username_transitions,
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

static struct parser_definition cool_protocol_parser_definition={
        .start_state=INITIAL_STATE,
        .states=cool_protocol_authentication_transitions,
        .states_count=N(cool_protocol_authentication_transitions),
        .states_n=state_transition_count
};

//functions

struct cool_protocol_authentication_message * init_cool_protocol_authentication_parser(){
    struct cool_protocol_authentication_message * new_cool_protocol_authentication_message = malloc(sizeof (struct cool_protocol_authentication_message));
    struct parser * cool_protocol_authentication_parser = parser_init(parser_no_classes(),&cool_protocol_parser_definition);
    new_cool_protocol_authentication_message->using_parser = cool_protocol_authentication_parser;
    new_cool_protocol_authentication_message->password_length=0;
    new_cool_protocol_authentication_message->password_characters_read=0;
    new_cool_protocol_authentication_message->username_characters_read=0;
    new_cool_protocol_authentication_message->username_length = 0;
    return new_cool_protocol_authentication_message;
}

 bool feed_cool_protocol_authentication_parser(struct cool_protocol_authentication_message * cool_protocol_data ,char * input,int input_size){
    const struct parser_event * current_event;
    for(int i = 0 ; i < input_size  && (cool_protocol_data->using_parser->state != END  ); i++){
        current_event = parser_feed(cool_protocol_data->using_parser,input[i]);
        uint8_t  current_character = current_event->data[0];
        switch (current_event->type) {
            case VERSION_READ_EVENT:
                handle_version_read_event(cool_protocol_data,current_character);
                break;
            case ULEN_READ_EVENT:
                handle_ulen_read_event(cool_protocol_data,current_character);
                break;
            case USERNAME_READ_EVENT:
                handle_username_read_event(cool_protocol_data, current_character);
                break;
            case PLEN_READ_EVENT:
                handle_plen_read_event(cool_protocol_data,current_character);
                break;
            case READING_PASSWORD_EVENT:
                handle_password_read_event(cool_protocol_data,current_character);
                break;
            case END :
//                end_parser_handler(cool_protocol_data,current_character);
                break;
            case ERROR_EVENT: 
            	handle_error_event(cool_protocol_data, current_character); return false;
        }
        if(current_event->type ==ERROR_EVENT)
            break;
    }

    if((cool_protocol_data->using_parser->state != END  ))
        return false;
    else return true;
}

void close_cool_protocol_authentication_parser(struct cool_protocol_authentication_message *  current_data){
    parser_destroy(current_data->using_parser);
    free(current_data->username);
    free(current_data->password);
    free(current_data);