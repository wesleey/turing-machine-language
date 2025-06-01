%option yylineno

%{
#include "ast.h"
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>

extern void yyerror(const char *s);
%}

%x MAX_DEPTH

%%

"@states"              { return DIRECTIVE_STATES; }
"@tape_alphabet"       { return DIRECTIVE_TAPE_ALPHABET; }
"@input_alphabet"      { return DIRECTIVE_INPUT_ALPHABET; }
"@initial_state"       { return DIRECTIVE_INITIAL_STATE; }
"@final_states"        { return DIRECTIVE_FINAL_STATES; }
"@max_depth"           { BEGIN(MAX_DEPTH); return DIRECTIVE_MAX_DEPTH; }

<MAX_DEPTH>":"         { return ':'; }
<MAX_DEPTH>[0-9]+      { BEGIN(INITIAL); yylval.ival = atoi(yytext); return INTEGER; }
<MAX_DEPTH>.           ;

"L"                    { yylval.str = strdup(yytext); return RESERVED_LEFT; }
"R"                    { yylval.str = strdup(yytext); return RESERVED_RIGHT; }
">"                    { yylval.str = strdup(yytext); return RESERVED_START; }
"_"                    { yylval.str = strdup(yytext); return RESERVED_BLANK; }

":"                    { return ':'; }
"("                    { return '('; }
")"                    { return ')'; }
","                    { return ','; }
"="                    { return '='; }

[a-zA-Z0-9]+           { yylval.str = strdup(yytext); return SYMBOL; }

"//".*                     ;
"/*"([^*]|\*+[^*/])*\*+"/" ;
[ \t\r\n]+                 ;

.                      { yyerror("Invalid or unexpected token"); }
