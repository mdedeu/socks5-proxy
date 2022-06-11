#include "metrics.h"

uint8_t add_user_handler(uint8_t ulen, uint8_t * username, uint8_t plen, uint8_t * password){
    return 255;
}

uint8_t remove_user_handler(uint8_t ulen, uint8_t * username){
    return 255;
}

uint8_t enable_spoofing_handler(uint8_t protocol){
    return 255;
}

uint8_t disable_spoofing_handler(uint8_t protocol){
    return 255;
}

uint64_t get_total_connections(){
    return 255;
}

uint64_t get_current_connections(){
    return 255;
}

uint64_t get_max_current_connections(){
    return 255;
}

uint64_t get_total_bytes_sent(){
    return 255;
}

uint64_t get_total_bytes_recv(){
    return 255;
}

uint64_t get_connected_users(){
    return 255;
}