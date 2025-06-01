#ifndef TABLE_H
#define TABLE_H

typedef struct node HTNode;
typedef struct table HTTable;

#define HT_INITIAL_SIZE 16
#define HT_RESIZE_INCREMENT 16

char* HT_prev_state(HTNode *node);
char HT_symbol(HTNode *node);
char* HT_next_state(HTNode *node);
char* HT_action(HTNode *node);
HTNode* HT_next(HTNode *node);

HTTable* HT_create();
void HT_insert(HTTable *table, const char *prev_state, char symbol, const char *next_state, const char *action);
HTNode* HT_search(HTTable *table, const char *prev_state, char symbol);
HTNode* HT_search_all(HTTable *table, const char *prev_state, char symbol);
void HT_print(HTTable *table);
void HT_free(HTTable *table);

#endif // TABLE_H
