#ifndef METRICS_H
#define METRICS_H

#include <stdint.h>

uint8_t add_user_handler(uint8_t ulen, uint8_t * username, uint8_t plen, uint8_t * password);

uint8_t remove_user_handler(uint8_t ulen, uint8_t * username);

uint8_t enable_spoofing_handler(uint8_t protocol);

uint8_t disable_spoofing_handler(uint8_t protocol);

uint64_t get_total_connections();

uint64_t get_current_connections();

uint64_t get_max_current_connections();

uint64_t get_total_bytes_sent();

uint64_t get_total_bytes_recv();

uint64_t get_connected_users();

#endif