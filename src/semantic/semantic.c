#include "semantic.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int semantic_error = 0;

extern HTTable *global_table;
extern char **states;
extern int states_count;
extern char **tape_alphabet;
extern int tape_alphabet_size;
extern char **input_alphabet;
extern int input_alphabet_size;
extern char *initial_state;
extern char **final_states;
extern int final_states_count;
extern int max_depth;

static int check_directive_name(const AST *node, const char *name) {
  const AST *symbol_node = AST_child(node, 0);
  return symbol_node && AST_symbol(symbol_node) &&
  strcmp(AST_symbol(symbol_node), name) == 0;
}

static void process_symbol_list(const AST *node, char ***dest_array, int *dest_size) {
  if (!node || AST_type(node) != AST_SYMBOLS) return;

  int count = AST_child_count(node);
  *dest_array = malloc(sizeof(char *) * count);
  *dest_size = count;

  for (int i = 0; i < count; ++i) {
    const AST *symbol_node = AST_child(node, i);
    (*dest_array)[i] = strdup(AST_symbol(symbol_node));
  }
}

static void process_states(const AST *node) {
  if (AST_child_count(node) != 2 || !check_directive_name(node, "STATES")) {
    fprintf(stderr, "Error: invalid '@states' directive.\n");
    semantic_error = 1;
    return;
  }
  process_symbol_list(AST_child(node, 1), &states, &states_count);
}

static void process_tape_alphabet(const AST *node) {
  if (AST_child_count(node) != 2 || !check_directive_name(node, "TAPE_ALPHABET")) {
    fprintf(stderr, "Error: invalid '@tape_alphabet' directive.\n");
    semantic_error = 1;
    return;
  }
  process_symbol_list(AST_child(node, 1), &tape_alphabet, &tape_alphabet_size);
}

static void process_input_alphabet(const AST *node) {
  if (AST_child_count(node) != 2 || !check_directive_name(node, "INPUT_ALPHABET")) {
    fprintf(stderr, "Error: invalid '@input_alphabet' directive.\n");
    semantic_error = 1;
    return;
  }
  process_symbol_list(AST_child(node, 1), &input_alphabet, &input_alphabet_size);
}

static void process_initial_state(const AST *node) {
  if (AST_child_count(node) != 2 || !check_directive_name(node, "INITIAL_STATE")) {
    fprintf(stderr, "Error: invalid '@initial_state' directive.\n");
    semantic_error = 1;
    return;
  }

  const AST *state_node = AST_child(node, 1);
  if (!AST_symbol(state_node)) {
    fprintf(stderr, "Error: missing symbol in '@initial_state'.\n");
    semantic_error = 1;
    return;
  }

  free(initial_state);
  initial_state = strdup(AST_symbol(state_node));
}

static void process_final_states(const AST *node) {
  if (AST_child_count(node) != 2 || !check_directive_name(node, "FINAL_STATES")) {
    fprintf(stderr, "Error: invalid '@final_states' directive.\n");
    semantic_error = 1;
    return;
  }
  process_symbol_list(AST_child(node, 1), &final_states, &final_states_count);
}

static void process_max_depth(const AST *node) {
  if (AST_child_count(node) != 2 || !check_directive_name(node, "MAX_DEPTH")) {
    fprintf(stderr, "Error: invalid '@max_depth' directive.\n");
    semantic_error = 1;
    return;
  }

  const AST *depth_node = AST_child(node, 1);
  int value = AST_value(depth_node);
  if (value <= 0) {
    fprintf(stderr, "Error: '@max_depth' must be an integer greater than 0. Got %d.\n", value);
    semantic_error = 1;
    return;
  }

  max_depth = value;
}

static void process_directive(const AST *node) {
  const AST *child = AST_child(node, 0);

  if (!child && !AST_symbol(child)) return;

  const char *directive = AST_symbol(child);

  if (strcmp(directive, "STATES") == 0)
    process_states(node);
  else if (strcmp(directive, "TAPE_ALPHABET") == 0)
    process_tape_alphabet(node);
  else if (strcmp(directive, "INPUT_ALPHABET") == 0)
    process_input_alphabet(node);
  else if (strcmp(directive, "INITIAL_STATE") == 0)
    process_initial_state(node);
  else if (strcmp(directive, "FINAL_STATES") == 0)
    process_final_states(node);
  else if (strcmp(directive, "MAX_DEPTH") == 0)
    process_max_depth(node);
}

static void process_transition(const AST *node) {
  if (AST_child_count(node) != 4) {
    fprintf(stderr, "Error: transition with incorrect number of children.\n");
    semantic_error = 1;
    return;
  }

  const char *from_state = AST_symbol(AST_child(node, 0));
  const char *read_symbol = AST_symbol(AST_child(node, 1));
  const char *to_state = AST_symbol(AST_child(node, 2));
  const char *action_symbol = AST_symbol(AST_child(node, 3));

  if (!from_state || !read_symbol || !to_state || !action_symbol) {
    fprintf(stderr, "Error: null symbol in transition.\n");
    semantic_error = 1;
    return;
  }

  HT_insert(global_table,
    strdup(from_state),
    read_symbol[0],
    strdup(to_state),
    strdup(action_symbol));
}

static void check_node(const AST *node) {
  if (!node) return;

  switch (AST_type(node)) {
    case AST_DIRECTIVE:
      process_directive(node);
      break;
    case AST_TRANSITION:
      process_transition(node);
      break;
    default:
      break;
  }

  for (int i = 0; i < AST_child_count(node); ++i)
  check_node(AST_child(node, i));
}

static int validate_required_directives(void) {
  if (!states || states_count == 0) {
    fprintf(stderr, "Error: '@states' directive is missing.\n");
    semantic_error = 1;
  }

  if (!tape_alphabet || tape_alphabet_size == 0) {
    fprintf(stderr, "Error: '@tape_alphabet' directive is missing.\n");
    semantic_error = 1;
  }

  if (!input_alphabet || input_alphabet_size == 0) {
    fprintf(stderr, "Error: '@input_alphabet' directive is missing.\n");
    semantic_error = 1;
  }

  if (!initial_state) {
    fprintf(stderr, "Error: '@initial_state' directive is missing.\n");
    semantic_error = 1;
  }

  if (!final_states || final_states_count == 0) {
    fprintf(stderr, "Error: '@final_states' directive is missing.\n");
    semantic_error = 1;
  }
}

int check_semantics(AST *root) {
  check_node(root);
  validate_required_directives();
  return semantic_error;
}
