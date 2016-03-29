#include "Cxt001Pass.h"

#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/raw_ostream.h"
///Mi primer doxygen
using namespace llvm;

char Cxt001Pass::ID = 0;

void Cxt001Pass::getAnalysisUsage(AnalysisUsage &AU) const {
  // Specifies that the pass will not invalidate any analysis already built on the IR
  AU.setPreservesAll();
  // Specifies that the pass will use the analysis LoopInfo
  AU.addRequiredTransitive<LoopInfoWrapperPass>();
}

bool Cxt001Pass::runOnFunction(Function &F) {
  LoopInfoWrapperPass &LIWP = getAnalysis<LoopInfoWrapperPass>();
  LoopInfo &LI = LIWP.getLoopInfo();
  unsigned op;
  for (Loop *L : LI) {
    totalLoops++;
  }

  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      op=I.getOpcode();
      switch(op) {
	case Instruction::Add:
	  adds++;
	  break;
	case Instruction::Sub:
	  subs++;
	  break;
	case Instruction::Mul:
	  muls++;
	  break;
	default:
	  others++;
	  break;
      }
      totalInstructions++;
    }
    totalBlocks++;
  }
  totalFunctions++;
  return false;
}

void Cxt001Pass::print(raw_ostream &O, const Module *M) const {
  O << "For module: " << M->getName() << "\n";
  O << "Number of functions: " << totalFunctions << "\n";
  O << "Number of adds: " << adds << "\n";
  O << "Number of subs: " << subs << "\n";
  O << "Number of multiplications: " << muls << "\n";
  O << "Number of loops: " << totalLoops << "\n";
  O << "Number of blocks: " << totalBlocks << "\n";
  O << "Number of blocks: " << totalBlocks << "\n";
  O << "Number of instructions: " << totalInstructions << "\n";
}
