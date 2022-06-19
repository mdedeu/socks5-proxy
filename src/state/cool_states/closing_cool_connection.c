#include "closing_cool_connection.h"



void closing_cool_connection_on_arrival(const unsigned state, struct selector_key * key){
    cool_timeout(key);
}