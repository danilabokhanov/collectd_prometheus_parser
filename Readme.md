# Prometheus Parser

## Deployment
```bison -d parser.y```
```flex lexer.l```
```gcc -o parser lex.yy.c parser.tab.c main.c ast.c -lfl```

## Tests

```./parser < test_metric.in > res.out```
