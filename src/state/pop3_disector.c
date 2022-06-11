#include "pop3_dissector.h"

static bool server_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size,enum exchange_status on_accepted, enum exchange_status on_deny);
static void pass_sent_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size);
static void user_sent_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size);
static void just_connected_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size);
static void user_accepted_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size);


pop3_dissector *  new_pop3_dissector() {
    struct pop3_dissector *  new_dissector = malloc(sizeof (struct pop3_dissector));
    new_dissector->status = JUST_CONNECTED;
    new_dissector->username =NULL;
    new_dissector->password = NULL ;
    return new_dissector;
}
void client_data(pop3_dissector * current_dissector, char * buffer , size_t buffer_size){
    switch (current_dissector->status) {
        case JUST_CONNECTED:
            just_connected_handler(current_dissector,buffer,buffer_size);
            break;
        case USER_ACCEPTED:
            user_accepted_handler(current_dissector,buffer,buffer_size);
            break;
        case PASS_ACCEPTED:
            printf("user credentials observed: %s -- %s\n", current_dissector->username,current_dissector->password );
            break;
        default:
            return;
    }
}


void origin_data(pop3_dissector *current_dissector , char * buffer , size_t buffer_size){
    switch (current_dissector->status) {
        case USER_SENT:
            user_sent_handler(current_dissector,buffer,buffer_size);
            break;
        case PASS_SENT:
            pass_sent_handler(current_dissector,buffer,buffer_size);
            break;
        default:
            return;
    }
}


static void just_connected_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size){
    if(current_dissector->data_message == NULL )
        current_dissector->data_message = init_pop3_data_parser("USER",5);
    if(current_dissector->username != NULL )
        free(current_dissector->username);

    bool finished = feed_pop3_data_parser(current_dissector->data_message,buffer,buffer_size);
    if(finished && current_dissector->data_message->connected){
        current_dissector->username = malloc(sizeof(current_dissector->data_message->data_characters_read));
        memcpy(current_dissector->username,current_dissector->data_message->data,current_dissector->data_message->data_characters_read);
        current_dissector->status = USER_SENT;
        close_pop3_data_parser(current_dissector->data_message);
    }
}

static void user_accepted_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size){
    if(current_dissector->data_message == NULL )
        current_dissector->data_message = init_pop3_data_parser("PASS",5);
    if(current_dissector->password != NULL )
        free(current_dissector->password);
    bool finished = feed_pop3_data_parser(current_dissector->data_message,buffer,buffer_size);
    if(finished && current_dissector->data_message->connected){
        current_dissector->password = malloc(sizeof(current_dissector->data_message->data_characters_read));
        memcpy(current_dissector->password,current_dissector->data_message->data,current_dissector->data_message->data_characters_read);
        current_dissector->status = PASS_SENT;
        close_pop3_data_parser(current_dissector->data_message);
    }
}


static void user_sent_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size){
    server_handler(current_dissector, buffer,buffer_size,USER_ACCEPTED, JUST_CONNECTED);
}



static void pass_sent_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size){
    server_handler(current_dissector, buffer,buffer_size,PASS_ACCEPTED, USER_SENT);
}


static bool server_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size,enum exchange_status on_accepted, enum exchange_status on_deny){
    if(current_dissector->ack_message==NULL)
        current_dissector->ack_message = init_pop3_connected_parser("+OK",4);

    bool finished = feed_pop3_connected_parser(current_dissector->ack_message,buffer,buffer_size);
    if(finished && current_dissector->ack_message->connected){
        current_dissector->status = on_accepted;
        close_pop3_connected_parser(current_dissector->ack_message);
    }else if ( finished && !current_dissector->ack_message->connected){
        current_dissector->status = on_deny;
        close_pop3_connected_parser(current_dissector->ack_message);
    }
}
