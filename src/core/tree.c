#include "tree.h"
#include "error.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  char *state;
  char *tape;
  int head;
  size_t tape_size;
  struct childNode *children;
};

struct childNode {
  struct node *child;
  struct childNode *next;
};

inline char* T_state(TNode *node) {
  return node->state;
}
inline char* T_tape(TNode *node) {
  return node->tape;
}
inline int T_head(TNode *node) {
  return node->head;
}
inline char T_symbol(TNode *node) {
  return node->tape[node->head];
}

static char* T_create_tape(const char *initial, size_t *size_out) {
  size_t len = strlen(initial);
  size_t alloc_size = ((len / T_TAPE_CHUNK) + 1) * T_TAPE_CHUNK;
  char *tape = (char*) malloc(alloc_size);

  if (tape == NULL)
    ERR_print("tape allocation failed.");

  strcpy(tape, initial);

  for (size_t i = len; i < alloc_size - 1; i++)
    tape[i] = '_';
  tape[alloc_size - 1] = '\0';

  *size_out = alloc_size;

  return tape;
}

static void T_ensure_tape_capacity(TNode *node, int head) {
  if (head < node->tape_size) return;

  size_t new_size = ((head / T_TAPE_CHUNK) + 1) * T_TAPE_CHUNK;
  char *new_tape = (char*) realloc(node->tape, new_size);

  if (new_tape == NULL)
    ERR_print("new tape reallocation failed.");

  node->tape = new_tape;

  for (size_t i = node->tape_size - 1; i < new_size - 1; i++)
    node->tape[i] = '_';
  node->tape[new_size - 1] = '\0';

  node->tape_size = new_size;
}

TNode* T_create(const char *state, const char *tape, int head) {
  if (head < 0)
    ERR_print("head position < 0");

  TNode *node = (TNode*) malloc(sizeof(TNode));

  if (node == NULL)
    ERR_print("tree node allocation failed.");

  node->state = strdup(state);
  node->tape = T_create_tape(tape, &node->tape_size);
  node->head = head;
  node->children = NULL;

  T_ensure_tape_capacity(node, head);

  return node;
}

void T_print(TNode *node, int depth) {
  if (!node) return;

  for (int i = 0; i < depth; i++) putchar(' ');
  printf("State: %s, Head: %d, Tape: %s\n",
         node->state,
         node->head,
         node->tape);
  TChildNode *current = node->children;
  while (current) {
    T_print(current->child, depth + 1);
    current = current->next;
  }
}

void T_free(TNode *node) {
  if (!node) return;
  TChildNode *current = node->children;
  while (current) {
    T_free(current->child);
    TChildNode *tmp = current;
    current = current->next;
    free(tmp);
  }
  free(node->tape);
  free(node);
}

static void T_add_child(TNode *parent, TNode *child) {
  TChildNode *new_child = (TChildNode*) malloc(sizeof(TChildNode));

  if (new_child == NULL)
    ERR_print("tree child node allocation failed.");

  new_child->child = child;
  new_child->next = parent->children;
  parent->children = new_child;
}

TNode* T_transition(TNode *parent, const char *next_state, const char *action) {
  if (!parent || !next_state || !action)
    return NULL;

  TNode *child = T_create(parent->state, parent->tape, parent->head);
  child->state = strdup(next_state);

  if (strcmp(action, C_RESERVED_RIGHT) == 0) {
    child->head++;
    T_ensure_tape_capacity(child, child->head);
  } else if (strcmp(action, C_RESERVED_LEFT) == 0 && child->head > 0) {
    child->head--;
  } else if (strlen(action) == 1) {
    T_ensure_tape_capacity(child, child->head);
    child->tape[child->head] = action[0];
  } else {
    ERR_printf("unknown action '%s'.", action);
  }

  T_add_child(parent, child);

  return child;
}
