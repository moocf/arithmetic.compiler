#include "mast.hpp"
#include <stdio.h>

using namespace std;
using namespace llvm;
extern Ast *yyroot;
extern int yyparse();
LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
Module TheModule("BASIC", TheContext);
map<string, Value*> NamedValues;


int main(int argc, char **argv) {
  printf("> ");
  yyparse();
  printf("%s\n", yyroot->str().c_str());
  FunctionType *FT = FunctionType::get(Type::getDoubleTy(TheContext), false);
  Function *F = Function::Create(FT, Function::ExternalLinkage, "main", &TheModule);
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
  Builder.SetInsertPoint(BB);
  Builder.CreateRet(yyroot->code());
  verifyFunction(*F);
  TheModule.print(errs(), nullptr);
  return 0;
}

void yyerror(const char *s) {
  fprintf(stderr, "error: %s\n", s);
}
