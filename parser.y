%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
    enum metric_type metric_type;
    metric* metric;
    label* label;
    timestamp* timestamp;
    comment* comment;
    type* type;
    help* help;
    node* node;
    tree* tree;
}

%token <string> NAME LABEL_VALUE COMMENT METRIC_HELP
%token <number> FLOAT_NUMBER
%token <integer> INTEGER_NUMBER
%token <metric_type> METRIC_TYPE
%token TYPE_DECLARATION HELP_DECLARATION OPEN_BRACE CLOSE_BRACE EQUALS COMMA
%type <number> numeric_value
%type <timestamp> timestamp
%type <metric> metric
%type <label> label label_list inner_label_list
%type <comment> comment
%type <type> type
%type <help> help
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
    | type {
        $$ = create_type_node($1);
    }
    | help {
        $$ = create_help_node($1);
    }
    ;

metric:
    NAME label_list numeric_value timestamp
    {
        $$ = create_metric($1, $2, $3, $4);
    }
    ;

numeric_value:
    FLOAT_NUMBER
    {
        $$ = $1;
    }
    | INTEGER_NUMBER
    {
        $$ = $1;
    }
    ;

timestamp:
    INTEGER_NUMBER
    {
        $$ = create_value_timestamp($1);
    }
    |
    {
        $$ = create_empty_timestamp();
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
    ;

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

type:
    TYPE_DECLARATION NAME METRIC_TYPE {
        $$ = create_type($2, $3);
    }

help:
    HELP_DECLARATION NAME METRIC_HELP {
        $$ = create_help($2, $3);
    }

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s near '%s'\n", yylineno, s, yytext);
    exit(EXIT_FAILURE);
}
