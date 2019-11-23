#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
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

  FunctionType *FT = FunctionType::get(Type::getFloatTy(TheContext), false);
  Function *F = Function::Create(FT, Function::ExternalLinkage, "expression", &TheModule);
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
  Builder.SetInsertPoint(BB);
  Builder.CreateRet(yyroot->code());
  verifyFunction(*F);
  TheModule.print(errs(), nullptr);

  auto TargetTriple = sys::getDefaultTargetTriple();
  InitializeAllTargetInfos();
  InitializeAllTargets();
  InitializeAllTargetMCs();
  InitializeAllAsmParsers();
  InitializeAllAsmPrinters();
  string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);
  auto CPU = "generic";
  auto Features = "";
  TargetOptions opt;
  auto RM = Optional<Reloc::Model>();
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
  TheModule.setDataLayout(TargetMachine->createDataLayout());
  TheModule.setTargetTriple(TargetTriple);
  auto Filename = "output.o";
  std::error_code EC;
  raw_fd_ostream dest(Filename, EC, sys::fs::F_None);
  legacy::PassManager pass;
  auto FileType = TargetMachine::CGFT_ObjectFile;
  if (TargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
    errs() << "TargetMachine can't emit a file of this type";
    return 1;
  }
  pass.run(TheModule);
  dest.flush();
  return 0;
}

void yyerror(const char *s) {
  fprintf(stderr, "error: %s\n", s);
}
