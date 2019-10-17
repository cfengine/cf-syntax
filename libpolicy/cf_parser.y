%code top {
#include <stdio.h>
#include <string.h>
}

%code requires {
#include "parse_lib.h"
}

%code {
extern char *yytext;

int yylex();

void yyerror(Parser *parser, const char *str)
{
    fprintf(stderr,"%s: '%s'\n", str, yytext);
    parser->errors += 1;
}

int yywrap()
{
    return 1;
}

extern char *yytext;

}

%param {Parser *parser}
%token SEMICOLON COMMA OPEN_CURLY CLOSE_CURLY FAT_ARROW BODY QUOTED_STRING IDENTIFIER
%token-table

%%
policy_file:
    /* empty */
    | body_list
    ;

body_list:
    body
    | body_list body
    ;

body:
    BODY
    IDENTIFIER
    IDENTIFIER
    body_body
    ;

body_body:
    OPEN_CURLY
    body_inner_body
    CLOSE_CURLY
    ;

body_inner_body:
    /* empty */
    | body_attribute_list
    ;

body_attribute_list:
    body_attribute
    | body_attribute_list body_attribute
    ;

body_attribute:
    IDENTIFIER
    FAT_ARROW
    value
    SEMICOLON
    ;

value:
    IDENTIFIER
    | QUOTED_STRING
    | list
    ;

list:
    OPEN_CURLY
    inner_list
    CLOSE_CURLY
    ;

inner_list:
    /* empty */
    | non_empty_list;

non_empty_list:
    value
    | non_empty_list COMMA value
    ;
%%

void print_token(int t) {
  printf("%d - %-16s - '%s'\n", t, yytname[YYTRANSLATE(t)], yytext);
}
