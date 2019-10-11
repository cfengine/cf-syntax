%{
#include <stdio.h>
#include <string.h>

#include <parse_lib.h>

extern char *yytext;

int yylex();

void yyerror(const char *str)
{
    fprintf(stderr,"%s: '%s'\n", str, yytext);
    parser_state.errors += 1;
}

int yywrap()
{
    return 1;
}


extern char *yytext;

%}

%token SEMICOLON COMMA OPEN_CURLY CLOSE_CURLY FAT_ARROW BODY QUOTED_STRING IDENTIFIER

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
