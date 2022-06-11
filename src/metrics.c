#include "metrics.h"

uint8_t add_user_handler(uint8_t ulen, uint8_t * username, uint8_t plen, uint8_t * password){
    return 0;
}

uint8_t remove_user_handler(uint8_t ulen, uint8_t * username){
    return 1;
}

uint8_t enable_spoofing_handler(uint8_t protocol){
    return 2;
}

uint8_t disable_spoofing_handler(uint8_t protocol){
    return 3;
}

uint64_t get_total_connections(){
    return 4;
}

uint64_t get_current_connections(){
    return 5;
}

uint64_t get_max_current_connections(){
    return 6;
}

uint64_t get_total_bytes_sent(){
    return 7;
}

uint64_t get_total_bytes_recv(){
    return 8;
}

uint64_t get_connected_users(){
    return 9;
}