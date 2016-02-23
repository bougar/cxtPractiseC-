#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"

class Cxt001Pass : public llvm::FunctionPass {
public:
  static char ID;

  Cxt001Pass()
      : FunctionPass(ID), totalFunctions(0), totalBlocks(0),
        totalInstructions(0), totalLoops(0), adds(0), muls(0),
	subs(0), others(0) {}

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

  bool runOnFunction(llvm::Function &F) override;

  void print(llvm::raw_ostream &O, const llvm::Module *M) const override;

private:
  int totalFunctions;
  int totalBlocks;
  int totalInstructions;
  int totalLoops;
  int adds;
  int muls;
  int subs;
  int others;
};
