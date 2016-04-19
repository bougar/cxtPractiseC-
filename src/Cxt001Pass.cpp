#include "Cxt001Pass.h"

#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/raw_ostream.h"
///Mi primer doxygen
using namespace llvm;
using namespace std;


class function_info_t {
	int funid = 0;
	std::string name = "";
	unsigned funOps = 0; //KPI_1: Operations per function counter
	public:
	void set_funid(int val){
		funid = val;
		}
	void set_name(std::string val){
		name = val;
		}
	void set_funOps(int val){
		funOps =val;
		}
	int get_funid(){ return funid;}
	std::string get_name(std::string val){return name;}
	unsigned get_funOps(int val){return funOps;}
	void increase_funOps(){funOps++;}
	void printthis(raw_ostream &O){
			O << "\tNº: " << funid;
			O << "  Function: " << name << "\n";
			O << "\tOperations: " << funOps << "\n\n";
		}
	
};



char Cxt001Pass::ID = 0;
struct fun_info{
	int funid;
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

bool Cxt001Pass::runOnFunction(Function &F) {
  LoopInfoWrapperPass &LIWP = getAnalysis<LoopInfoWrapperPass>();
  LoopInfo &LI = LIWP.getLoopInfo();
  
  fun_info newtoken_KPI_1; //Element for funOpVector
  newtoken_KPI_1.funid = funCounter;
  funCounter++;
  newtoken_KPI_1.name = F.getName();
  newtoken_KPI_1.funOps = 0; //KPI_1: Operations per function counter
  
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
      newtoken_KPI_1.funOps++; //KPI_1: Operations per function counter
    }
    totalBlocks++;
  }
  functionOperationsVector.push_back(newtoken_KPI_1); //Insert in funOpVector 
  totalFunctions++;
  
  return false;
}

void print_kpi_1(raw_ostream &O){
	fun_info aux;
	int i, l= functionOperationsVector.size();
	O << "Number of instructions per function: \n\n";
	O << "**************************************************\n";
	for(i=0; i<l; i++){
		
		}
	O << "**************************************************\n";
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
  O << "Number of instructions: " << totalInstructions << "\n\n";
  O << "Featured KPI's: \n\n"; 
  print_kpi_1(O);
}
