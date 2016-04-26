#include "FunctionInfo.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include <tuple>
using namespace std;

class Cxt001Pass : public llvm::FunctionPass {
public:
  static char ID;

	Cxt001Pass() : llvm::FunctionPass(ID),funCounter(1) {};


  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

  bool runOnFunction(llvm::Function &F) override;

  void print(llvm::raw_ostream &O, const llvm::Module *M) const override;
	void printTotals();

	
	private:
		vector<FunctionInfo> functionOperationsVector;	
		int funCounter;
		vector<Memoria> memoryVector;
};
