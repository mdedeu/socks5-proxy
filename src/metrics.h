#ifndef METRICS_H
#define METRICS_H
#include <stdint.h>
#include <stdbool.h>
#include "string.h"
#include <stdlib.h>
#define MAX_USERS 10
#define ADMIN_NUMBERS 4

struct server_user_info{
    char * username;
    char * password;
    bool connected;
};

struct metrics{
    uint64_t  historical_total_connections;
    uint64_t  current_connections;
    bool password_dissector_enable;
    uint64_t max_current_connections;
    uint64_t bytes_sent;
    uint64_t bytes_received;
    uint8_t  registered_clients;
    struct server_user_info * client_users[MAX_USERS];
    struct server_user_info admin_users[ADMIN_NUMBERS];
    bool clients_need_authentication;
};


bool add_user_handler(uint8_t ulen, uint8_t * username, uint8_t plen, uint8_t * password);
bool remove_user_handler(uint8_t ulen, uint8_t * username);
bool enable_spoofing_handler(/*uint8_t protocol*/);
bool disable_spoofing_handler(/*uint8_t protocol*/);
void increment_current_connections();
void decrement_current_connections();
bool clients_need_authentication();

void increment_data_sent(uint64_t data_quantity);
void increment_data_received(uint64_t data_quantity);


uint8_t change_buffer_size_handler(uint16_t size);

uint64_t get_total_connections();
uint64_t get_current_connections();
uint64_t get_max_current_connections();
uint64_t get_total_bytes_sent();
uint64_t get_total_bytes_recv();
uint64_t get_connected_users();
void set_clients_need_authentication(bool boolean);

bool connect_user(char * username , char * password);
void disconnect(char * username);

uint64_t get_max_buffer_size();

uint64_t get_user_list();

#endif