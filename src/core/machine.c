#include "machine.h"
#include "queue.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct machine {
  char *initial_state;
  HTTable* table;
  char **final_states;
  int final_count;
};

Machine* M_create(HTTable* table, const char* initial_state, char** final_states, int final_count) {
  Machine* machine = (Machine*) malloc(sizeof(Machine));

  if (machine == NULL)
    ERR_print("machine allocation failed.");

  machine->table = table;
  machine->initial_state = strdup(initial_state);
  machine->final_states = final_states;
  machine->final_count = final_count;

  return machine;
}

static int is_final_state(Machine* machine, const char* state) {
  for (int i = 0; i < machine->final_count; i++)
    if (strcmp(machine->final_states[i], state) == 0)
      return 1;
  return 0;
}

void M_run(Machine* machine, const char* input, int max_depth) {
  size_t len = strlen(input);
  char* tape = malloc(len + 3);

  if (tape == NULL) {
    fprintf(stderr, "Tape allocation failure.\n");
    return;
  }

  strcpy(tape, ">_");
  strcat(tape, input);

  TNode* root = T_create(machine->initial_state, tape, 0);
  Queue* queue = Q_create();

  Q_enqueue(queue, root, 0);

  while (!Q_is_empty(queue)) {
    int depth;
    TNode* current = Q_dequeue(queue, &depth);

    printf("Level %d: State: %s, Head: %d, Tape: %s\n",
           depth, T_state(current), T_head(current), T_tape(current));

    if (is_final_state(machine, T_state(current))) {
      printf("Reached final state: %s. Halting.\n", T_state(current));
      break;
    }

    if (depth >= max_depth)
      continue;

    char symbol = T_symbol(current);
    HTNode* transitions = HT_search_all(machine->table, T_state(current), symbol);

    for (HTNode* t = transitions; t != NULL; t = HT_next(t)) {
      TNode* child = T_transition(current, HT_next_state(t), HT_action(t));
      Q_enqueue(queue, child, depth + 1);
    }

    while (transitions) {
      HTNode* next = HT_next(transitions);
      free(transitions);
      transitions = next;
    }
  }

  Q_free(queue);
  T_free(root);
}

void M_free(Machine* machine) {
  free(machine->initial_state);
  free(machine);
}
