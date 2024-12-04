%{
#include "ast.h"
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int yywrap(void) {
    return 1;
}
%}

%%
"# TYPE" {perror("METRIC_TYPE"); return METRIC_TYPE;}
"# HELP" {perror("METRIC_HELP"); return METRIC_HELP;}
"# "[^\n]* {yylval.string = strndup(yytext + 2, strlen(yytext) - 2);  perror("COMMENT"); return COMMENT;}
\"[^"]*\" {yylval.string = strndup(yytext + 1, strlen(yytext) - 2); perror("LABEL_VALUE"); return LABEL_VALUE;}
[0-9]+\.[0-9]+ {yylval.number = atof(yytext); perror("FLOAT_NUMBER"); return FLOAT_NUMBER;}
[0-9]+ {yylval.integer = strtoll(yytext, NULL, 10); perror("INTEGER_NUMBER"); return INTEGER_NUMBER;}
"{" {perror("OPEN_BRACE"); return OPEN_BRACE;}
"}" {perror("CLOSE_BRACE"); return CLOSE_BRACE;}
= {perror("EQUALS"); return EQUALS;}
, {perror("COMMA"); return COMMA;}
[a-zA-Z_][a-zA-Z0-9_]* {yylval.string = strdup(yytext); perror("NAME"); perror(yylval.string); return NAME;}
[ \t\r]+ {perror("NOTHING");}
"\n" {yylineno++; perror("NEW LINE");}
. {perror("UNMATCHED");}
%%
