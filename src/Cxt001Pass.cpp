#include "Cxt001Pass.h"

#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/raw_ostream.h"
///Mi primer doxygen
using namespace llvm;
using namespace std;

char Cxt001Pass::ID = 0;


void Cxt001Pass::getAnalysisUsage(AnalysisUsage &AU) const {
  // Specifies that the pass will not invalidate any analysis already built on the IR
  AU.setPreservesAll();
  // Specifies that the pass will use the analysis LoopInfo
  AU.addRequiredTransitive<LoopInfoWrapperPass>();
}

void countAllocates(CallInst &call,FunctionInfo &info){
	Function * f= (call.getCalledFunction());
	string str = f->getName();
	if (str=="malloc")
		info.mem.mallocs++;
	if (str=="free")
		info.mem.frees++;
}

bool Cxt001Pass::runOnFunction(Function &F) {
  LoopInfoWrapperPass &LIWP = getAnalysis<LoopInfoWrapperPass>();
  LoopInfo &LI = LIWP.getLoopInfo();
  
  FunctionInfo functionInfo; //Element for funOpVector
  functionInfo.setFunId( funCounter );
  funCounter++;
  functionInfo.setName ( F.getName() );
  functionInfo.setFunOps( 0 ); //KPI_1: Operations per function counter
  
  unsigned op;

  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      op=I.getOpcode();
	    switch(op) {
				case Instruction::Call:
					if ( CallInst *call = dyn_cast<CallInst>(&I) )
						countAllocates(*call,functionInfo);
				break;
				case Instruction::FMul:
					functionInfo.f.fmul++;
					functionInfo.f.ftotals++;
				break;
				case Instruction::FAdd:
					functionInfo.f.fadd++;
					functionInfo.f.ftotals++;
				break;
				case Instruction::FDiv:
					functionInfo.f.fdiv++;
					functionInfo.f.ftotals++;
				break;
				case Instruction::FRem:
					functionInfo.f.frem++;
					functionInfo.f.ftotals++;
				break;
				case Instruction::FSub:
					functionInfo.f.fsub++;
					functionInfo.f.ftotals++;
				break;
				case Instruction::FCmp:
					functionInfo.f.fcmp++;
					functionInfo.f.ftotals++;
				break;
				default:
				break;
      }
      functionInfo.increaseFunOps(); //KPI_1: Operations per function counter
    }
  }
  functionOperationsVector.push_back(functionInfo); //Insert in funOpVector 
  return false;
}

/*
void print_kpi_1(raw_ostream &O){
	fun_info aux;
	int i, l= functionOperationsVector.size();
	O << "Number of instructions per function: \n\n";
	O << "**************************************************\n";
	for(i=0; i<l; i++){
			aux = functionOperationsVector.at(i);
			O << "\tNº: " << aux.funid;
			O << "  Function: " << aux.name << "\n";
			O << "\tOperations: " << aux.funOps << "\n\n";
			O << "\tmallocs Number: " << aux.mallocs << "\n\n";
			O << "\tfrees Number: " << aux.frees << "\n\n";

		}
	O << "**************************************************\n";
	}
*/
void Cxt001Pass::print(raw_ostream &O, const Module *M) const {
  O << "For module: " << M->getName() << "\n";
}
