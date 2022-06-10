#include "cool_state_machine.h"

static const struct state_definition authenticate_reading_state= {.state=COOL_AUTHENTICATE_READING, .on_arrival=NULL,.on_read_ready=NULL,.on_departure=NULL};

static const struct state_definition states[] = {
    authenticate_reading_state
    /*
    authenticate_writing_state,
    request_reading,
    add_user_reading,
    remove_user_reading,
    enable_password_spoofing_reading,
    disable_password_spoofing_reading,
    response_writing
    */
};

static struct state_machine sock_client_machine = {
        .initial=COOL_AUTHENTICATE_READING,
        .states=states,
        .max_state = N(states)
};

struct state_machine *init_copl_state_machine() {
    struct state_machine *aux = malloc(sizeof(state_machine));
    memcpy(aux, &sock_client_machine, sizeof(state_machine));
    stm_init(aux);
    return aux;
}

void destroy_cool_state(struct state_machine *sock_machine) {
    free(sock_machine);
}
