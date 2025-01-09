bison -d -o parser.c parser.y && flex -o scanner.c scanner.l && gcc -o parser scanner.c parser.c main.c ast.c log.c -lfl
