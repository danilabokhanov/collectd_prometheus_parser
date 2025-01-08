bison -d parser.y && flex lexer.l && gcc -o parser lex.pr.c parser.tab.c main.c ast.c log.c -lfl
