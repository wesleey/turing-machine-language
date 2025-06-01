#ifndef AST_H
#define AST_H

typedef enum {
  AST_START,
  AST_DIRECTIVES,
  AST_DIRECTIVE,
  AST_TRANSITIONS,
  AST_TRANSITION,
  AST_SYMBOLS,
  AST_SYMBOL,
  AST_INTEGER
} AST_node_type;

typedef struct ast AST;

AST_node_type AST_type(const AST *node);
char* AST_symbol(const AST *node);
int AST_value(const AST *node);
AST* AST_child(const AST *parent, int i);
int AST_child_count(const AST *node);

AST* AST_create(AST_node_type type);
void add_child(AST *parent, AST *child);
AST *AST_create_symbol(const char *symbol);
AST *AST_create_integer(int value);
void AST_free(AST *node);
void AST_print(const AST *node, int indent);

#endif // AST_H
