#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>

extern int yyparse(void);
extern void set_lexer_buffer(const char *str);

extern tree *tr;

char *read_str_from_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Can't open file");
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  long sz = ftell(file);
  rewind(file);
  char *res = (char *)malloc(sz + 1);
  if (!res) {
    perror("Couldn't allocate memory for lexer buffer\n");
    return NULL;
  }
  fread(res, sizeof(char), sz, file);
  res[sz] = '\0';
  fclose(file);
  return res;
}

int main(int argc, char *argv[]) {
  const char *lexer_input = read_str_from_file(argv[1]);
  set_lexer_buffer(lexer_input);
  if (yyparse() == 0) {
    printf("Parsing completed successfully.\n\n");
    print_tree(tr);
    delete_tree(tr);
  } else {
    printf("Failed while parsing\n");
  }
  return 0;
}
