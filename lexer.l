%{
#include "ast.h"
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

static YY_BUFFER_STATE lexer_buffer;

void set_lexer_buffer(const char* str) {
    if (lexer_buffer) {
        yy_delete_buffer(lexer_buffer);
    }
    lexer_buffer = yy_scan_string(str);
    yy_switch_to_buffer(lexer_buffer);
}

int lexer_error(const char* s, int status_code) {
    fprintf(stderr, "Unrecognized token at line %d: %s", yylineno, s);
    return status_code;
}

int yywrap(void) {
    return 1;
}

char* replace_escaped_chars(char* text, size_t text_size) {
    char* res = calloc(text_size + 1, sizeof(*res));
    if (!res) {
        return NULL;
    }
    size_t res_iter = 0;
    for (size_t i = 0; i < text_size; i++) {
        if (text[i] == '\\') {
            if (i + 1 >= text_size) {
                break;
            }
            switch (text[i + 1]) {
            case ('\\'): {
                res[res_iter++] = '\\';
                break;
            }
            case ('"'): {
                res[res_iter++] = '"';
                break;
            }
            case ('n'): {
                res[res_iter++] = '\n';
                break;
            }
            case ('r'): {
                res[res_iter++] = '\r';
                break;
            }
            case ('t'): {
                res[res_iter++] = '\t';
                break;
            }
            default: {
                break;
            }
            }
            i++;
        } else {
            res[res_iter++] = text[i];
        }
    }
    res[res_iter] = '\0';
    return res;
}

%}

%x COMMENT_TEXT
%x TYPE
%x HELP_NAME
%x HELP_HINT

%%
NaN {
    yylval.number = NAN;
    perror("FLOAT_NUMBER");
    return FLOAT_NUMBER;
}

\+Inf {
    yylval.number = INFINITY;
    perror("FLOAT_NUMBER");
    return FLOAT_NUMBER;
}

\-Inf {
    yylval.number = -INFINITY;
    perror("FLOAT_NUMBER");
    return FLOAT_NUMBER;
}

"# TYPE" {
    BEGIN(TYPE);
    perror("TYPE_DECLARATION");
    return TYPE_DECLARATION;
}

"# HELP" {
    BEGIN(HELP_NAME);
    perror("HELP_DECLARATION");
    return HELP_DECLARATION;
}

<TYPE>"counter" {
    BEGIN(INITIAL);
    yylval.metric_type = COUNTER;
    perror("METRIC_TYPE");
    return METRIC_TYPE;
}

<TYPE>"gauge" {
    BEGIN(INITIAL);
    yylval.metric_type = GAUGE;
    perror("METRIC_TYPE");
    return METRIC_TYPE;
}

<TYPE>"histogram" {
    BEGIN(INITIAL);
    yylval.metric_type = HISTOGRAM;
    perror("METRIC_TYPE");
    return METRIC_TYPE;
}

<TYPE>"summary" {
    BEGIN(INITIAL);
    yylval.metric_type = SUMMARY;
    perror("METRIC_TYPE");
    return METRIC_TYPE;
}

<TYPE>"untyped" {
    BEGIN(INITIAL);
    yylval.metric_type = UNTYPED;
    perror("METRIC_TYPE");
    return METRIC_TYPE;
}

<TYPE>[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.string = strdup(yytext);
    perror("NAME");
    perror(yylval.string);
    return NAME;
}

"#" {
    BEGIN(COMMENT_TEXT);
    perror("COMMENT_DECLARATION");
}

<COMMENT_TEXT>[^\n]* {
    BEGIN(INITIAL);
    yylval.string = strndup(yytext + 1, strlen(yytext) - 1);
    perror("COMMENT");
    return COMMENT;
}

<HELP_NAME>[a-zA-Z_][a-zA-Z0-9_]* {
    BEGIN(HELP_HINT);
    yylval.string = strdup(yytext);
    perror("NAME");
    perror(yylval.string);
    return NAME;
}

<HELP_HINT>[^\n]* {
    BEGIN(INITIAL);
    yylval.string = strndup(yytext + 1, strlen(yytext) - 1); 
    perror("METRIC_HELP");
    return METRIC_HELP;
}

\"(\\[\"\\nrt]|[^\"\\])*\" {
    yylval.string = replace_escaped_chars(yytext + 1, strlen(yytext) - 2);
    if (yylval.string == NULL) {
        return lexer_error("Couldn't allocate memory for string without escaped chars\n", EXIT_FAILURE);
    }
    perror("LABEL_VALUE");
    perror(yylval.string);
    return LABEL_VALUE;
}

[+-]?[0-9]+\.[0-9]+([eE][+-]?[0-9]+)? {
    yylval.number = atof(yytext);
    perror("FLOAT_NUMBER");
    return FLOAT_NUMBER;
}

[+-]?[0-9]+ {
    yylval.integer = strtoll(yytext, NULL, 10);
    perror("INTEGER_NUMBER");
    return INTEGER_NUMBER;
}

"{" {
    perror("OPEN_BRACE");
    return OPEN_BRACE;
}

"}" {
    perror("CLOSE_BRACE");
    return CLOSE_BRACE;
}

= {
    perror("EQUALS");
    return EQUALS;
}

, {
    perror("COMMA");
    return COMMA;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.string = strdup(yytext);
    perror("NAME");
    perror(yylval.string);
    return NAME;
}

[ \t\r]+ {
    perror("NOTHING");
}

"\n" {
    yylineno++;
    perror("NEW_LINE");
}

. {
    perror("UNMATCHED");
}
%%
