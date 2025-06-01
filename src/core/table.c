#include "table.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  char* prev_state;
  char symbol;
  char* next_state;
  char* action;
  struct node *next;
};

struct table {
  struct node **table;
  int size;
  int count;
};

inline char* HT_prev_state(HTNode *node) {
  return node->prev_state;
}
inline char HT_symbol(HTNode *node) {
  return node->symbol;
}
inline char* HT_next_state(HTNode *node) {
  return node->next_state;
}
inline char* HT_action(HTNode *node) {
  return node->action;
}
inline HTNode* HT_next(HTNode *node) {
  return node->next;
}

static int HT_hash_function(const char *state, char symbol, int size) {
  unsigned int hash = 0;
  while (*state) hash = (hash * 31 + *state++) % size;
  hash = (hash * 31 + symbol) % size;
  return hash;
}

static HTNode* HT_create_node(const char *prev_state, const char symbol, const char *next_state, const char *action) {
  HTNode *node = (HTNode*) malloc(sizeof(HTNode));

  if (node == NULL)
    ERR_print("hash table node allocation failed.");

  node->prev_state = strdup(prev_state);
  node->next_state = strdup(next_state);
  node->symbol = symbol;
  node->action = strdup(action);
  node->next = NULL;

  return node;
}

HTTable* HT_create() {
  HTTable *table = (HTTable*) malloc(sizeof(HTTable));

  if (table == NULL)
    ERR_print("hash table allocation failed.");

  table->size = HT_INITIAL_SIZE;
  table->count = 0;
  table->table = (HTNode**) malloc(sizeof(HTNode*) * table->size);

  if (table->table == NULL)
    ERR_print("table->table allocation failed.");

  for (int i = 0; i < table->size; i++)
    table->table[i] = NULL;

  return table;
}

static void HT_resize(HTTable *table) {
  int new_size = table->size + HT_RESIZE_INCREMENT;

  HTNode **new_table = (HTNode**) malloc(sizeof(HTNode*) * new_size);

  if (new_table == NULL)
    ERR_print("new table allocation failed.");

  for (int i = 0; i < new_size; i++)
    new_table[i] = NULL;

  for (int i = 0; i < table->size; i++) {
    HTNode *current = table->table[i];
    while (current) {
      HTNode *next = current->next;
      int new_index = HT_hash_function(current->prev_state, current->symbol, new_size);
      current->next = new_table[new_index];
      new_table[new_index] = current;
      current = next;
    }
  }

  free(table->table);
  table->table = new_table;
  table->size = new_size;
}

void HT_insert(HTTable *table, const char *prev_state, char symbol, const char *next_state, const char *action) {
  if (table->count >= table->size)
    HT_resize(table);

  int index = HT_hash_function(prev_state, symbol, table->size);

  HTNode *new_node = HT_create_node(prev_state, symbol, next_state, action);
  new_node->next = table->table[index];
  table->table[index] = new_node;
  table->count++;
}

HTNode* HT_search(HTTable *table, const char *prev_state, char symbol) {
  int index = HT_hash_function(prev_state, symbol, table->size);
  HTNode *current = table->table[index];

  while (current) {
    if (current->symbol == symbol && strcmp(current->prev_state, prev_state) == 0)
      return current;
    current = current->next;
  }

  return NULL;
}

HTNode* HT_search_all(HTTable *table, const char *prev_state, char symbol) {
  int index = HT_hash_function(prev_state, symbol, table->size);
  HTNode* current = table->table[index];

  HTNode* head = NULL;
  HTNode* tail = NULL;

  while (current) {
    if (current->symbol == symbol && strcmp(current->prev_state, prev_state) == 0) {
      HTNode* copy = HT_create_node(current->prev_state, current->symbol, current->next_state, current->action);
      copy->next = NULL;
      if (head == NULL) {
        head = tail = copy;
      } else {
        tail->next = copy;
        tail = copy;
      }
    }
    current = current->next;
  }

  return head;
}

void HT_print(HTTable *table) {
  printf("Hash Table (%d elements / size %d):\n", table->count, table->size);
  for (int i = 0; i < table->size; i++) {
    HTNode *current = table->table[i];
    if (current) {
      printf("  [%d]:\n", i);
      while (current) {
        printf("    (%s, %c) = (%s, %s)\n",
               current->prev_state,
               current->symbol,
               current->next_state,
               current->action);
        current = current->next;
      }
    }
  }
}

void HT_free(HTTable *table) {
  for (int i = 0; i < table->size; i++) {
    HTNode *current = table->table[i];
    while (current) {
      HTNode *next = current->next;
      free(current);
      current = next;
    }
  }

  free(table->table);
  free(table);
}
