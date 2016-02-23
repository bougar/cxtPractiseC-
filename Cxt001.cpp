#include <iostream>

#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/LegacyPassNameParser.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/InitializePasses.h"
#include "llvm/LinkAllIR.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"

#include <Cxt001Pass.h>

using namespace std;
using namespace llvm;

std::unique_ptr<Module> getModuleFromFile(const char *path) {
  LLVMContext &Context = llvm::getGlobalContext();
  SMDiagnostic Err;

  auto result = parseIRFile(path, Err, Context);

  if (Err.getSourceMgr()) {
    cerr << Err.getMessage().str();
    exit(-1);
  }
  return result;
}

void initializeLLVM(int argc, char *argv[]) {
  sys::PrintStackTraceOnErrorSignal();
  llvm::PrettyStackTraceProgram X(argc, argv);

  // Initialize passes
  PassRegistry &Registry = *PassRegistry::getPassRegistry();
  initializeCore(Registry);
  initializeAnalysis(Registry);
}

int main(int argc, char *argv[]) {
  llvm_shutdown_obj Y; // Call llvm_shutdown() on exit.

  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <llvm assemble/bitcode file>\n";
    return -1;
  }

  initializeLLVM(argc, argv);
  legacy::PassManager pm;

  auto Mod = getModuleFromFile(argv[1]);

  // Needs to be a pointer because PassManager will delete it
  Cxt001Pass *pass = new Cxt001Pass();
  pm.add(pass);
  pm.run(*Mod);

  pass->print(llvm::dbgs(), Mod.get());

  return 0;
}
