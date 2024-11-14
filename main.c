#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>

int yyparse();
extern tree *tr;

void yyerror(const char *s) { fprintf(stderr, "Error: %s\n", s); }

int main() {
  if (yyparse() == 0) {
    printf("Parsing completed successfully.\n");
    print_tree(tr);
    delete_tree(tr);
  } else {
    perror("Failed while parsing\n");
  }
  return 0;
}
