%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ast.h"

extern int yylex();
void yyerror(const char* s);

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

%token <string> DECLARATION LABEL_VALUE COMMENT
%token <number> METRIC_VALUE
%token <integer> TIMESTAMP
%token OPEN_BRACE CLOSE_BRACE EQUALS COMMA

%type <metric> metric
%type <label> label label_list inner_label_list
%type <comment> comment
%type <node> node
%type <tree> tree

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
        if ($1 -> tp == METRIC_NODE) {
            add_metric_to_tree(tr, $1 -> body.metric);
        } else if ($1 -> tp == COMMENT_NODE) {
            add_comment_to_tree(tr, $1 -> body.comment);
        }
    }
    | node tree
    {
        if ($1 -> tp == METRIC_NODE) {
            add_metric_to_tree($2, $1 -> body.metric);
        } else if ($1 -> tp == COMMENT_NODE) {
            add_comment_to_tree($2, $1 -> body.comment);
        }
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
    DECLARATION label_list METRIC_VALUE TIMESTAMP
    {
        $$ = create_metric($1, $2, $3, $4);
    }
    |
    DECLARATION label_list METRIC_VALUE
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
    DECLARATION EQUALS LABEL_VALUE
    {
        $$ = create_label($1, $3);
    }
    ;
%%
