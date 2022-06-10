#include "cool_state_machine.h"

static const struct state_definition authenticate_reading_state= {.state=COOL_AUTHENTICATE_READING, .on_arrival=cool_authenticate_reading_arrival,.on_read_ready=cool_authenticate_read_handler,.on_departure=cool_authenticate_reading_departure};
static const struct state_definition authenticate_writing_state= {.state=COOL_AUTHENTICATE_WRITING, .on_arrival=cool_authenticate_writing_arrival,.on_read_ready=cool_authenticate_write_handler,.on_departure=cool_authenticate_writing_departure};

static const struct state_definition states[] = {
    authenticate_reading_state,
    authenticate_writing_state,
    request_reading_state,
    /*
    response_writing_state
    */
};

static struct state_machine cool_client_machine = {
        .initial=COOL_AUTHENTICATE_READING,
        .states=states,
        .max_state = N(states)
};

struct state_machine *init_cool_state_machine() {
    struct state_machine *aux = malloc(sizeof(state_machine));
    memcpy(aux, &cool_client_machine, sizeof(state_machine));
    stm_init(aux);
    return aux;
}

void destroy_cool_state(struct state_machine *cool_machine) {
    free(cool_machine);
}
