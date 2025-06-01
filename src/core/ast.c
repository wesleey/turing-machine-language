#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ast {
  AST_node_type type;
  char *symbol;
  int value;
  struct ast **children;
  int child_count;
  int child_capacity;
};

inline AST_node_type AST_type(const AST *node) {
  return node->type;
}
inline char* AST_symbol(const AST *node) {
  return node->symbol;
}
inline int AST_value(const AST *node) {
  return node->value;
}
inline AST* AST_child(const AST *parent, int i) {
  return parent->children[i];
}
inline int AST_child_count(const AST *parent) {
  return parent->child_count;
}

AST* AST_create(AST_node_type type) {
  AST *node = malloc(sizeof(AST));
  node->type = type;
  node->symbol = NULL;
  node->value = 0;
  node->children = NULL;
  node->child_count = 0;
  node->child_capacity = 0;
  return node;
}

AST* AST_create_symbol(const char *symbol) {
  AST *node = AST_create(AST_SYMBOL);
  node->symbol = strdup(symbol);
  return node;
}

AST* AST_create_integer(int value) {
  AST *node = AST_create(AST_INTEGER);
  node->value = value;
  return node;
}

void add_child(AST *parent, AST *child) {
  if (!parent || !child) return;

  if (parent->child_capacity == 0) {
    parent->child_capacity = 4;
    parent->children = malloc(sizeof(AST*) * parent->child_capacity);
  } else if (parent->child_count >= parent->child_capacity) {
    parent->child_capacity *= 2;
    parent->children = realloc(parent->children, sizeof(AST*) * parent->child_capacity);
  }

  parent->children[parent->child_count++] = child;
}

void AST_print(const AST *node, int indent) {
  if (!node) return;

  for (int i = 0; i < indent; i++) printf("  ");

  switch (node->type) {
    case AST_START:
      printf("START\n");
      break;
    case AST_DIRECTIVES:
      printf("DIRECTIVES\n");
      break;
    case AST_DIRECTIVE:
      printf("DIRECTIVE\n");
      break;
    case AST_TRANSITIONS:
      printf("TRANSITIONS\n");
      break;
    case AST_TRANSITION:
      printf("TRANSITION\n");
      break;
    case AST_SYMBOLS:
      printf("SYMBOLS\n");
      break;
    case AST_SYMBOL:
      printf("SYMBOL: %s\n", node->symbol);
      break;
    case AST_INTEGER:
      printf("INTEGER: %d\n", node->value);
      break;
  }

  for (int i = 0; i < node->child_count; i++)
    AST_print(node->children[i], indent + 1);
}

void AST_free(AST *node) {
  if (!node) return;

  for (int i = 0; i < node->child_count; i++)
    AST_free(node->children[i]);

  free(node->children);
  free(node->symbol);
  free(node);
}
