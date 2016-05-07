#include "FunctionInfo.h"
#include "llvm/Pass.h"
#include "llvm/ADT/Triple.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include <tuple>
using namespace std;

class Cxt001Pass : public llvm::FunctionPass {
public:
  static char ID;

	Cxt001Pass(const llvm::Module * m) : llvm::FunctionPass(ID),funCounter(1) {
		module = m; 
		llvm::Twine twine=llvm::Twine(m->getTargetTriple());
		triple = llvm::Triple(twine);
	};


  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

  bool runOnFunction(llvm::Function &F) override;

  void print(llvm::raw_ostream &O, const llvm::Module *M) const override;
	void printTotals();
	void printFirstTable();

	
	private:
		const llvm::Module * module;
		llvm::Triple triple;
		vector<FunctionInfo> functionOperationsVector;	
		int funCounter;
		vector<Memoria> memoryVector;
};
