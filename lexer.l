%{
#include "ast.h"
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int yywrap(void) {
    return 1;
}
%}

%x COMMENT_TEXT
%x TYPE
%x HELP_NAME
%x HELP_HINT

%%
NaN {yylval.number = NAN; perror("FLOAT_NUMBER"); return FLOAT_NUMBER;}
\+Inf {yylval.number = INFINITY; perror("FLOAT_NUMBER"); return FLOAT_NUMBER;}
\-Inf {yylval.number = -INFINITY; perror("FLOAT_NUMBER"); return FLOAT_NUMBER;}
"# TYPE" {BEGIN(TYPE); perror("TYPE_DECLARATION"); return TYPE_DECLARATION;}
"# HELP" {BEGIN(HELP_NAME); perror("HELP_DECLARATION"); return HELP_DECLARATION;}
<TYPE>"counter" {BEGIN(INITIAL); yylval.metric_type = COUNTER; perror("METRIC_TYPE"); return METRIC_TYPE;}
<TYPE>"gauge" {BEGIN(INITIAL); yylval.metric_type = GAUGE; perror("METRIC_TYPE"); return METRIC_TYPE;}
<TYPE>"histogram" {BEGIN(INITIAL); yylval.metric_type = HISTOGRAM; perror("METRIC_TYPE"); return METRIC_TYPE;}
<TYPE>"summary" {BEGIN(INITIAL); yylval.metric_type = SUMMARY; perror("METRIC_TYPE"); return METRIC_TYPE;}
<TYPE>"untyped" {BEGIN(INITIAL); yylval.metric_type = UNTYPED; perror("METRIC_TYPE"); return METRIC_TYPE;}
<TYPE>[a-zA-Z_][a-zA-Z0-9_]* {yylval.string = strdup(yytext); perror("NAME"); perror(yylval.string); return NAME;}
"#" {BEGIN(COMMENT_TEXT); perror("COMMENT_DECLARATION");}
<COMMENT_TEXT>[^\n]* {BEGIN(INITIAL); yylval.string = strndup(yytext + 1, strlen(yytext) - 1); perror("COMMENT"); return COMMENT;}
<HELP_NAME>[a-zA-Z_][a-zA-Z0-9_]* {BEGIN(HELP_HINT); yylval.string = strdup(yytext); perror("NAME"); perror(yylval.string); return NAME;}
<HELP_HINT>[^\n]* {BEGIN(INITIAL); yylval.string = strndup(yytext + 1, strlen(yytext) - 1);  perror("METRIC_HELP"); return METRIC_HELP;}
\"([^\"\\]|\\.)*\" {yylval.string = strndup(yytext + 1, strlen(yytext) - 2); perror("LABEL_VALUE"); perror(yylval.string); return LABEL_VALUE;}
[+-]?[0-9]+\.[0-9]+([eE][+-]?[0-9]+)? {yylval.number = atof(yytext); perror("FLOAT_NUMBER"); return FLOAT_NUMBER;}
[+-]?[0-9]+ {yylval.integer = strtoll(yytext, NULL, 10); perror("INTEGER_NUMBER"); return INTEGER_NUMBER;}
"{" {perror("OPEN_BRACE"); return OPEN_BRACE;}
"}" {perror("CLOSE_BRACE"); return CLOSE_BRACE;}
= {perror("EQUALS"); return EQUALS;}
, {perror("COMMA"); return COMMA;}
[a-zA-Z_][a-zA-Z0-9_]* {yylval.string = strdup(yytext); perror("NAME"); perror(yylval.string); return NAME;}
[ \t\r]+ {perror("NOTHING");}
"\n" {yylineno++; perror("NEW LINE");}
. {perror("UNMATCHED");}
%%
