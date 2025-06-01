#ifndef MACHINE_H
#define MACHINE_H

#include "table.h"

typedef struct machine Machine;

Machine* M_create(HTTable* table, const char* initial_state, char** final_states, int final_count);
void M_run(Machine* machine, const char* input, int max_steps);
void M_free(Machine* machine);

#endif // MACHINE_H
