#ifndef TREE_H
#define TREE_H

#define T_TAPE_CHUNK 16

typedef struct node TNode;
typedef struct childNode TChildNode;

char* T_state(TNode *node);
char* T_tape(TNode *node);
int T_head(TNode *node);
char T_symbol(TNode *node);

TNode* T_create(const char *state, const char *tape, int head);
void T_print(TNode *node, int depth);
void T_free(TNode *node);
TNode* T_transition(TNode *parent, const char *new_state, const char *action);

#endif // TREE_H
