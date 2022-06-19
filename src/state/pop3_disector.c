#include "pop3_dissector.h"

static void server_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size,enum exchange_status on_accepted, enum exchange_status on_deny);
static void pass_sent_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size);
static void user_sent_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size);
static void just_connected_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size);
static void user_accepted_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size);
static void clean_data_parser(pop3_dissector * current_dissector);
static void clean_ack_parser(pop3_dissector * current_dissector);
static char * USER = "USER";
static char * PASS = "PASS";


pop3_dissector *  new_pop3_dissector() {
    struct pop3_dissector *  new_dissector = malloc(sizeof (struct pop3_dissector));
    memset(new_dissector, 0, sizeof(struct pop3_dissector));
    new_dissector->status = JUST_CONNECTED;
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
        default:
            return;
    }
}


bool origin_data(pop3_dissector *current_dissector , char * buffer , size_t buffer_size){
    switch (current_dissector->status) {
        case USER_SENT:
            user_sent_handler(current_dissector,buffer,buffer_size);
            break;
        case PASS_SENT:
            pass_sent_handler(current_dissector,buffer,buffer_size);
            break;
        default:
            return false;
    }
    if(current_dissector->status == PASS_ACCEPTED){
        return true;
    }
    return false;
}

bool is_tracing_conversation(pop3_dissector * current_dissector){
    return current_dissector->status != PASS_ACCEPTED;
}



static void just_connected_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size){
        if(current_dissector->data_message == NULL )
        current_dissector->data_message = init_pop3_data_parser(USER, 4);

    if(current_dissector->username != NULL ){
        free(current_dissector->username);
        current_dissector->username = NULL;
    }

    bool finished = feed_pop3_data_parser(current_dissector->data_message,buffer,buffer_size);

    if(finished && current_dissector->data_message->connected){
        current_dissector->username = malloc(current_dissector->data_message->data_characters_read  +1 );
        memcpy(current_dissector->username,current_dissector->data_message->data,current_dissector->data_message->data_characters_read);
        current_dissector->status = USER_SENT;
        clean_data_parser(current_dissector);
    }else if(finished && !current_dissector->data_message->connected){
        clean_data_parser(current_dissector);
    }

}

static void user_accepted_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size){

        if(current_dissector->data_message == NULL )
        current_dissector->data_message = init_pop3_data_parser(PASS,4);

    if(current_dissector->password != NULL ){
        free(current_dissector->password);
        current_dissector->password =NULL;
    }

    struct pop3_data_message * user_resending_username = init_pop3_data_parser(USER,4);

    bool finished = feed_pop3_data_parser(current_dissector->data_message,buffer,buffer_size);
    bool resending = feed_pop3_data_parser(user_resending_username,buffer,buffer_size);

    if(finished && current_dissector->data_message->connected){
        current_dissector->password = malloc(current_dissector->data_message->data_characters_read + 1 );
        memcpy(current_dissector->password,current_dissector->data_message->data,current_dissector->data_message->data_characters_read);
        current_dissector->status = PASS_SENT;
        clean_data_parser(current_dissector);
    }else if(finished && !current_dissector->data_message->connected){
        clean_data_parser(current_dissector);
    }

    if(resending && user_resending_username->connected){ //username has been sent again
        if(current_dissector->username != NULL) free(current_dissector->username);
        current_dissector->username = calloc(1,user_resending_username->data_characters_read  +1 );
        memcpy(current_dissector->username,user_resending_username->data,user_resending_username->data_characters_read);
        current_dissector->status = USER_SENT;
        clean_data_parser(current_dissector);
    }

}




static void user_sent_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size){
    server_handler(current_dissector, buffer,buffer_size,USER_ACCEPTED, JUST_CONNECTED);
}



static void pass_sent_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size){
    server_handler(current_dissector, buffer,buffer_size,PASS_ACCEPTED, JUST_CONNECTED);
}


static void server_handler(pop3_dissector *  current_dissector, char * buffer , size_t buffer_size,enum exchange_status on_accepted, enum exchange_status on_deny){
    if(current_dissector->ack_message==NULL)
        current_dissector->ack_message = init_pop3_connected_parser("+OK",3);

    bool finished = feed_pop3_connected_parser(current_dissector->ack_message,buffer,buffer_size);

    if(finished && current_dissector->ack_message->connected){
        current_dissector->status = on_accepted;
        clean_ack_parser(current_dissector);
    }else if ( finished && !current_dissector->ack_message->connected){
        current_dissector->status = on_deny;
        clean_ack_parser(current_dissector);
        clean_data_parser(current_dissector);
    }

}

void destroy_dissector(pop3_dissector * removing){
    if(removing == NULL) return;
    if(removing->password!=NULL) free(removing->password);
    if(removing->username!=NULL) free(removing->username);
    clean_data_parser(removing);
    clean_ack_parser(removing);
    free(removing);
}


static void clean_data_parser(pop3_dissector * current_dissector){
    if(current_dissector->data_message!=NULL){
        close_pop3_data_parser(current_dissector->data_message);
        current_dissector->data_message = NULL;
    }
}

static void clean_ack_parser(pop3_dissector * current_dissector){
    if(current_dissector->ack_message != NULL ){
        close_pop3_connected_parser(current_dissector->ack_message);
        current_dissector->ack_message = NULL;
    }
}