%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ast.h"

extern int yylex();
void yyerror(const char* s);

extern int yylineno;
extern char *yytext;
tree* tr = NULL;
%}

%union {
    char* string;
    double number;
    int64_t integer;
    metric* metric;
    label* label;
    comment* comment;
    node* node;
    tree* tree;
}

%token <string> NAME LABEL_VALUE COMMENT
%token <number> FLOAT_NUMBER
%token <integer> INTEGER_NUMBER
%token METRIC_TYPE METRIC_HELP OPEN_BRACE CLOSE_BRACE EQUALS COMMA
%type <metric> metric
%type <label> label label_list inner_label_list
%type <comment> comment
%type <node> node
%type <tree> tree

%define parse.error detailed

%%

input:
    tree {
        tr = $1;
    }
    ;

tree:
    node
    {
        $$ = create_empty_tree();
        add_node_to_tree($$, $1);
    }
    | node tree
    {
        add_node_to_tree($2, $1);
        $$ = $2;
    }
    ;

node:
    metric
    {
        $$ = create_metric_node($1);
    }
    | comment {
        $$ = create_comment_node($1);
    }
    ;

metric:
    NAME label_list FLOAT_NUMBER INTEGER_NUMBER
    {
        $$ = create_metric($1, $2, $3, $4);
    }
    |
    NAME label_list FLOAT_NUMBER
    {
        $$ = create_metric($1, $2, $3, -1);
    }
    |
    NAME label_list INTEGER_NUMBER INTEGER_NUMBER
    {
        $$ = create_metric($1, $2, $3, $4);
    }
    |
    NAME label_list INTEGER_NUMBER
    {
        $$ = create_metric($1, $2, $3, -1);
    }
    ;

comment:
    COMMENT
    {
        $$ = create_comment($1);
    }
    ;

label_list:
    OPEN_BRACE inner_label_list CLOSE_BRACE
    {
        $$ = $2;
    }
    |
    {
        $$ = NULL;
    }

inner_label_list:
    label {
        $$ = $1;
    }
    | label COMMA inner_label_list {
        $$ =  add_label_to_list($3, $1);
    }
    |
    {
        $$ = NULL;
    }
    ;

label:
    NAME EQUALS LABEL_VALUE
    {
        $$ = create_label($1, $3);
    }
    ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s near '%s'\n", yylineno, s, yytext);
    exit(EXIT_FAILURE);
}
