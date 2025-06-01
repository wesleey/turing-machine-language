%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

AST *root;
int yylex(void);
void yyerror(const char *s);

extern int yylineno;
%}

%union {
    char *str;
    int ival;
    AST *ast;
}

%token <str> SYMBOL RESERVED_START RESERVED_BLANK RESERVED_LEFT RESERVED_RIGHT
%token <ival> INTEGER
%token DIRECTIVE_STATES
%token DIRECTIVE_TAPE_ALPHABET
%token DIRECTIVE_INPUT_ALPHABET
%token DIRECTIVE_INITIAL_STATE
%token DIRECTIVE_FINAL_STATES
%token DIRECTIVE_MAX_DEPTH

%type <ast> start directives directive transitions transition symbols symbol

%%

start:
    directives transitions {
        root = AST_create(AST_START);
        add_child(root, $1);
        add_child(root, $2);
        $$ = root;
    }
;

directives:
    directives directive {
        $$ = $1;
        add_child($$, $2);
    }
    | directive {
        $$ = AST_create(AST_DIRECTIVES);
        add_child($$, $1);
    }
;

directive:
    DIRECTIVE_STATES ':' symbols {
        $$ = AST_create(AST_DIRECTIVE);
        add_child($$, AST_create_symbol("STATES"));
        add_child($$, $3);
    }
    | DIRECTIVE_TAPE_ALPHABET ':' symbols {
        $$ = AST_create(AST_DIRECTIVE);
        add_child($$, AST_create_symbol("TAPE_ALPHABET"));
        add_child($$, $3);
    }
    | DIRECTIVE_INPUT_ALPHABET ':' symbols {
        $$ = AST_create(AST_DIRECTIVE);
        add_child($$, AST_create_symbol("INPUT_ALPHABET"));
        add_child($$, $3);
    }
    | DIRECTIVE_INITIAL_STATE ':' symbol {
        $$ = AST_create(AST_DIRECTIVE);
        add_child($$, AST_create_symbol("INITIAL_STATE"));
        add_child($$, $3);
    }
    | DIRECTIVE_FINAL_STATES ':' symbols {
        $$ = AST_create(AST_DIRECTIVE);
        add_child($$, AST_create_symbol("FINAL_STATES"));
        add_child($$, $3);
    }
    | DIRECTIVE_MAX_DEPTH ':' INTEGER {
        $$ = AST_create(AST_DIRECTIVE);
        add_child($$, AST_create_symbol("MAX_DEPTH"));
        add_child($$, AST_create_integer($3));
    }
;

transitions:
    transitions transition {
        $$ = $1;
        add_child($$, $2);
    }
    | transition {
        $$ = AST_create(AST_TRANSITIONS);
        add_child($$, $1);
    }
;

transition:
    '(' symbol ',' symbol ')' '=' '(' symbol ',' symbol ')' {
        $$ = AST_create(AST_TRANSITION);
        add_child($$, $2);
        add_child($$, $4);
        add_child($$, $8);
        add_child($$, $10);
    }
;

symbols:
    symbols ',' symbol {
        $$ = $1;
        add_child($$, $3);
    }
    | symbol {
        $$ = AST_create(AST_SYMBOLS);
        add_child($$, $1);
    }
;

symbol:
    SYMBOL {
        $$ = AST_create_symbol($1);
    }
    | RESERVED_START {
        $$ = AST_create_symbol($1);
    }
    | RESERVED_BLANK {
        $$ = AST_create_symbol($1);
    }
    | RESERVED_LEFT {
        $$ = AST_create_symbol($1);
    }
    | RESERVED_RIGHT {
        $$ = AST_create_symbol($1);
    }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s.\n", yylineno, s);
}
