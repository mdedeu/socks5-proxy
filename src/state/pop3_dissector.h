//
// Created by shadad on 11/06/22.
//

#ifndef SOCKS5_PROXY_POP3_DISECTOR_H
#define SOCKS5_PROXY_POP3_DISECTOR_H
#include "../parsing/pop3_data_parser.h"
#include "../parsing/pop3_connected_parser.h"
#include <stdlib.h>

enum exchange_status{
    JUST_CONNECTED,
    USER_SENT,
    USER_ACCEPTED,
    PASS_SENT,
    PASS_ACCEPTED
};

typedef struct pop3_dissector{
    enum exchange_status status;
    char * username ;
    char * password  ;
    struct pop3_data_message * data_message;
    struct pop3_connected_message * ack_message;
} pop3_dissector;


pop3_dissector  * new_pop3_dissector();
void client_data(pop3_dissector * current_dissector, char * buffer , size_t buffer_size);
bool origin_data(pop3_dissector * current_dissector , char * buffer , size_t buffer_size);
bool is_tracing_conversation(pop3_dissector * current_dissector);
void destroy_dissector(pop3_dissector * removing);




#endif //SOCKS5_PROXY_POP3_DISECTORADT_H
