%{
#include "ast.h"
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#undef ECHO
#define ECHO

static YY_BUFFER_STATE lexer_buffer;

void set_lexer_buffer(const char* str) {
    if (lexer_buffer) {
        yy_delete_buffer(lexer_buffer);
    }
    lexer_buffer = yy_scan_string(str);
    yy_switch_to_buffer(lexer_buffer);
}

int lexer_error(const char* s, int status_code) {
    fprintf(stderr, "Unrecognized token at line %d: %s", prlineno, s); // ERROR LEVEL
    return status_code;
}

int prwrap(void) {
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

%option prefix="pr"

%x COMMENT_TEXT
%x TYPE
%x HELP_NAME
%x HELP_HINT

%%
NaN {
    prlval.number = NAN;
    return FLOAT_NUMBER;
}

\+Inf {
    prlval.number = INFINITY;
    return FLOAT_NUMBER;
}

\-Inf {
    prlval.number = -INFINITY;
    return FLOAT_NUMBER;
}

"# TYPE" {
    BEGIN(TYPE);
    return TYPE_DECLARATION;
}

"# HELP" {
    BEGIN(HELP_NAME);
    return HELP_DECLARATION;
}

<TYPE>"counter" {
    BEGIN(INITIAL);
    prlval.metric_type = PR_COUNTER;
    return METRIC_TYPE;
}

<TYPE>"gauge" {
    BEGIN(INITIAL);
    prlval.metric_type = PR_GAUGE;
    return METRIC_TYPE;
}

<TYPE>"histogram" {
    BEGIN(INITIAL);
    prlval.metric_type = PR_HISTOGRAM;
    return METRIC_TYPE;
}

<TYPE>"summary" {
    BEGIN(INITIAL);
    prlval.metric_type = PR_SUMMARY;
    return METRIC_TYPE;
}

<TYPE>"untyped" {
    BEGIN(INITIAL);
    prlval.metric_type = PR_UNTYPED;
    return METRIC_TYPE;
}

<TYPE>[a-zA-Z_][a-zA-Z0-9_]* {
    prlval.string = strdup(prtext);
    if (!prlval.string) {
        return EXIT_FAILURE;
    }
    return NAME;
}

"#" {
    BEGIN(COMMENT_TEXT);
}

<COMMENT_TEXT>[^\n]* {
    BEGIN(INITIAL);
    prlval.string = strndup(prtext + 1, strlen(prtext) - 1);
    if (!prlval.string) {
        return EXIT_FAILURE;
    }
    return COMMENT;
}

<HELP_NAME>[a-zA-Z_][a-zA-Z0-9_]* {
    BEGIN(HELP_HINT);
    prlval.string = strdup(prtext);
    if (!prlval.string) {
        return EXIT_FAILURE;
    }
    return NAME;
}

<HELP_HINT>[^\n]* {
    BEGIN(INITIAL);
    prlval.string = strndup(prtext + 1, strlen(prtext) - 1);
    if (!prlval.string) {
        return EXIT_FAILURE;
    }
    return METRIC_HELP;
}

\"(\\[\"\\nrt]|[^\"\\])*\" {
    prlval.string = replace_escaped_chars(prtext + 1, strlen(prtext) - 2);
    if (prlval.string == NULL) {
        return lexer_error("Couldn't allocate memory for string without escaped chars\n", EXIT_FAILURE);
    }
    return LABEL_VALUE;
}

[+-]?[0-9]+\.[0-9]+([eE][+-]?[0-9]+)? {
    prlval.number = atof(prtext);
    return FLOAT_NUMBER;
}

[+-]?[0-9]+ {
    prlval.integer = strtoll(prtext, NULL, 10);
    return INTEGER_NUMBER;
}

"{" {
    return OPEN_BRACE;
}

"}" {
    return CLOSE_BRACE;
}

= {
    return EQUALS;
}

, {
    return COMMA;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    prlval.string = strdup(prtext);
    if (!prlval.string) {
        return EXIT_FAILURE;
    }
    return NAME;
}

[ \t\r]+ {}

"\n" {
    prlineno++;
}

. {}
%%
