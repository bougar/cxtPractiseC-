#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"

class Cxt001Pass : public llvm::FunctionPass {
public:
  static char ID;

  Cxt001Pass()
      : FunctionPass(ID) {};

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

  bool runOnFunction(llvm::Function &F) override;

  void print(llvm::raw_ostream &O, const llvm::Module *M) const override;

};
