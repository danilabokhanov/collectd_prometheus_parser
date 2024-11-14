#include "ast.h"
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

%%{
    machine lexer;

    action store_declaration {
        yylval.string = strdup(yytext);
        return DECLARATION;
    }

    action store_label_value {
        yylval.string = strndup(yytext + 1, strlen(yytext) - 2);
        return LABEL_VALUE;
    }

    action store_metric_value {
        yylval.number = atof(yytext);
        return METRIC_VALUE;
    }

    action store_metric_timestamp {
        yyval.integer = strtoll(yytext, NULL, 10);
        return TIMESTAMP;
    }

    action store_comment {
        yyval.string = strdup(yytext);
        return COMMENT;
    }

    main := |*
        '#' [^\n]* => { store_comment; };
        '\"' [^"]* '\"' => { store_label_value; };
        [0-9]+('.'[0-9]+)? => {store_metric_value; };
        [0-9]+ => {store_metric_name; };
        '{' => {return OPEN_BRACE; };
        '}' => {return CLOSE_BRACE; };
        '=' => {return EQUALS; };
        ',' => {return COMMA; };
        [a-zA-Z_][a-zA-Z0-9_]* => {store_declaration; };
        [ \t\n\r]+;
    *|;
}%%

