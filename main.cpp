#include "mast.hpp"
#include <stdio.h>

using namespace std;
extern Ast *yyroot;
extern int yyparse();


int main(int argc, char **argv) {
  printf("> ");
  yyparse();
  printf("%s\n", yyroot->str().c_str());
  return 0;
}

void yyerror(const char *s) {
  fprintf(stderr, "error: %s\n", s);
}
