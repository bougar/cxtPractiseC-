#include "Cxt001Pass.h"
#include <iostream>

#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/raw_ostream.h"
///Mi primer doxygen
using namespace llvm;
using namespace std;

struct flotantes {
	int fadd;
	int fmul;
	int fsub;
	int fdiv;
	int frem;
	int fcmp;
	int ftotals;
};

char Cxt001Pass::ID = 0;
struct fun_info{
	flotantes f;
	int funid;
	int mallocs=0;
	int frees=0;
	std::string name;
	unsigned funOps; //KPI_1: Operations per function counter
};
int funCounter = 1;
std::vector<fun_info> functionOperationsVector;

void Cxt001Pass::getAnalysisUsage(AnalysisUsage &AU) const {
  // Specifies that the pass will not invalidate any analysis already built on the IR
  AU.setPreservesAll();
  // Specifies that the pass will use the analysis LoopInfo
  AU.addRequiredTransitive<LoopInfoWrapperPass>();
}

void countAllocates(CallInst &call,fun_info &info){
	Function * f= (call.getCalledFunction());
	string str = f->getName();
	if (str=="malloc")
		info.mallocs++;
	if (str=="free")
		info.frees++;
}

bool Cxt001Pass::runOnFunction(Function &F) {
  LoopInfoWrapperPass &LIWP = getAnalysis<LoopInfoWrapperPass>();
  LoopInfo &LI = LIWP.getLoopInfo();
  
  fun_info newtoken_KPI_1; //Element for funOpVector
  newtoken_KPI_1.funid = funCounter;
  funCounter++;
  newtoken_KPI_1.name = F.getName();
  newtoken_KPI_1.funOps = 0; //KPI_1: Operations per function counter
  
  unsigned op;

  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      op=I.getOpcode();
	    switch(op) {
				case Instruction::Call:
					if ( CallInst *call = dyn_cast<CallInst>(&I) )
						countAllocates(*call,newtoken_KPI_1);
				break;
				case Instruction::FMul:
					newtoken_KPI_1.f.fmul++;
					newtoken_KPI_1.f.ftotals++;
				break;
				case Instruction::FAdd:
					newtoken_KPI_1.f.fadd++;
					newtoken_KPI_1.f.ftotals++;
				break;
				case Instruction::FDiv:
					newtoken_KPI_1.f.fdiv++;
					newtoken_KPI_1.f.ftotals++;
				break;
				case Instruction::FRem:
					newtoken_KPI_1.f.frem++;
					newtoken_KPI_1.f.ftotals++;
				break;
				case Instruction::FSub:
					newtoken_KPI_1.f.fsub++;
					newtoken_KPI_1.f.ftotals++;
				break;
				case Instruction::FCmp:
					newtoken_KPI_1.f.fcmp++;
					newtoken_KPI_1.f.ftotals++;
				break;
				default:
				break;
      }
      newtoken_KPI_1.funOps++; //KPI_1: Operations per function counter
    }
  }
  functionOperationsVector.push_back(newtoken_KPI_1); //Insert in funOpVector 
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
