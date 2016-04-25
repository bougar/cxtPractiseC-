#include "Cxt001Pass.h"
#include <iostream>
#include "llvm/IR/Value.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Analysis/MemoryBuiltins.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Function.h"
#include <tuple>
///Mi primer doxygen
using namespace llvm;
using namespace std;
void imprimir (string a){
	cout << a << "\n";
}

char Cxt001Pass::ID = 0;
Value * aux=NULL;
tuple<Value *,Value *> auxFree = tuple<Value *,Value *>(NULL,NULL); 

void Cxt001Pass::getAnalysisUsage(AnalysisUsage &AU) const {
  // Specifies that the pass will not invalidate any analysis already built on the IR
  AU.setPreservesAll();
  // Specifies that the pass will use the analysis LoopInfo
  AU.addRequiredTransitive<LoopInfoWrapperPass>();
}
Value * extractFreeValueFromLoad(Value * val){
	if (const CallInst * call = dyn_cast<CallInst>(val) ){
		if ( (call->getOperand(0))->hasName() );
		if (call->getArgOperand(0) == std::get<0>(auxFree)){
			string a;
			a = (get<1>(auxFree))->getName();
			cout << "Free: " << a << "\n";
		}
	}
	return NULL;
}

Value * extractMallocValueFromStore(Instruction &I){
	if ( StoreInst * store = dyn_cast<StoreInst>(&I) ){
		if ( aux != NULL )
			if ( aux == (store->getOperand(0) ) ) {
				aux=NULL;
				string a;
				a = store->getOperand(1)->getName();
				cout << "Malloc: " << a << "\n";  
				return store->getOperand(1);
			}
	}
	aux=NULL;
	return NULL;
}
/// Get the variable which store malloc pointer result
/// Then we will mathc it with the next store instrucction because this
/// does not get the real variable. Just a temporary one
Value * extractValue(Value *val,TargetLibraryInfo targetLibraryInfo){
	//tips: Look for the variable in the next store operation.
	//(We need the bitcast temp variable).
	if (const CallInst * call = dyn_cast<CallInst>(val) ){
		aux = static_cast<Value *>(val);
		for (Value::const_user_iterator begin = call->user_begin(), end = call->user_end();
			begin != end;){
				if (const BitCastInst *bitCastInst = dyn_cast<BitCastInst>(*begin++)) {
					//We need store next value and math with the next store intructution in order
					//to get a math with the variable
					aux = static_cast<Value *>(const_cast<BitCastInst *>(bitCastInst));
				}
			}
	}
	return NULL; 
}

void countAllocates(Value &val,FunctionInfo &info, TargetLibraryInfo &targetLibraryInfo){
	//We need to know if it is a new or a malloc
	if ( isMallocLikeFn( &val,&targetLibraryInfo,false ) ){
		extractValue(&val,targetLibraryInfo);
		info.mem.mallocs++;
	}
	//We need to know if it is a free or a delete function
	if ( isFreeCall ( &val,&targetLibraryInfo ) ){
		extractFreeValueFromLoad(&val);
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
				case Instruction::BitCast:
					if ( std::get<0>(auxFree) )
						if ( BitCastInst * bitCastInst = dyn_cast<BitCastInst>(&I))
							if ( std::get<0>(auxFree) == bitCastInst->llvm::User::getOperand(0) )
								std::get<0>(auxFree) = static_cast<Value *>(const_cast<BitCastInst *>(bitCastInst)); 		
				break;
				case Instruction::Load:
					if (LoadInst * load = dyn_cast<LoadInst>(&I)){ 
						std::get<0>(auxFree) = static_cast<Value *>(const_cast<LoadInst *>(load));
						std::get<1>(auxFree) = load->getOperand(0);
					}
				break;
				case Instruction::Store:
					extractMallocValueFromStore(I);
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
}
