#include "Cxt001Pass.h"
#include <iostream>
#include "llvm/IR/Value.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Analysis/MemoryBuiltins.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/raw_ostream.h"
///Mi primer doxygen
using namespace llvm;
using namespace std;

int a;
string b;
Value *c=NULL;
char Cxt001Pass::ID = 0;


void Cxt001Pass::getAnalysisUsage(AnalysisUsage &AU) const {
  // Specifies that the pass will not invalidate any analysis already built on the IR
  AU.setPreservesAll();
  // Specifies that the pass will use the analysis LoopInfo
  AU.addRequiredTransitive<LoopInfoWrapperPass>();
}
Value * extractValue(Value *val,TargetLibraryInfo targetLibraryInfo){
	const CallInst &call = *(extractMallocCall(val,&targetLibraryInfo));
	for (Value::const_user_iterator UI = call.user_begin(),E = call.user_end();UI != E;){
		if ( const Value *val =dyn_cast <Value>(*UI++)){
			if (const BitCastInst *BCI = dyn_cast<BitCastInst>(val)) {
				c=const_cast<Value *>(val);
				b=val->getName();
				cout << b << "\n";
			}	
		}
	}
	return NULL;
}

void countAllocates(Value &val,FunctionInfo &info, TargetLibraryInfo &targetLibraryInfo){
	if ( isMallocLikeFn( &val,&targetLibraryInfo,false ) ){
		extractValue(&val,targetLibraryInfo);
		info.mem.mallocs++;
	}
	if ( isFreeCall ( &val,&targetLibraryInfo ) ){
		info.mem.frees++;
	}
	 	
}
bool Cxt001Pass::runOnFunction(Function &F) {
  FunctionInfo functionInfo; //Element for funOpVector
  functionInfo.setFunId( funCounter );
  funCounter++;
  functionInfo.setName ( F.getName() );
  functionInfo.setFunOps( 0 ); //KPI_1: Operations per function counter
  unsigned op;
  for (BasicBlock &BB : F) {
		Module *M=BB.getModule();
		Twine twine=Twine(M->getTargetTriple());
		Triple t = Triple(twine);
		TargetLibraryInfoImpl targetLibraryInfoImpl = TargetLibraryInfoImpl(t);
		TargetLibraryInfo targetLibraryInfo = TargetLibraryInfo(targetLibraryInfoImpl);
    for (Instruction &I : BB) {
      op=I.getOpcode();
	    switch(op) {
				case Instruction::Call:
					if ( Value *value = dyn_cast<Value>(&I) )
						countAllocates(*value,functionInfo,targetLibraryInfo);
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
				case Instruction::Store:
					 if (StoreInst *a = dyn_cast<StoreInst>(&I)){
						 Value * v=a->getOperand(0);
						if ( v == c )
							cout << "culooooo";
						else 
							cout << "no hubo suerte";
					}


					
				default:
				break; 
		} 
	functionInfo.increaseFunOps(); //KPI_1: Operations per function counter
    }
  }
  functionOperationsVector.push_back(functionInfo); //Insert in funOpVector 
  return false;
}

void Cxt001Pass::print(raw_ostream &O, const Module *M) const {
  O << "For module: " << M->getName() << "\n";
  O << "a value: " << b << "\n";
  O << "a value: " << a << "\n";
}
