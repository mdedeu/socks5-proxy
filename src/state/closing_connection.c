#include "closing_connection.h"



void closing_connection_on_arrival(const unsigned state ,struct selector_key * key ){
    socks5_timeout(key);
}