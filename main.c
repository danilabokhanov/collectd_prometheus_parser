#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>

int yyparse(void);

extern tree *tr;
extern FILE *yyin;

int main(int argc, char *argv[]) {
  if (yyparse() == 0) {
    printf("Parsing completed successfully.\n\n");
    print_tree(tr);
    delete_tree(tr);
  } else {
    printf("Failed while parsing\n");
  }

  if (argc > 1) {
    fclose(yyin);
  }
  return 0;
}
