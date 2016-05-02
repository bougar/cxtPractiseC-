#include "Cxt001Pass.h"
#include "llvm/IR/Value.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Analysis/MemoryBuiltins.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Function.h"
#include <tuple>
#include <iostream>

using namespace llvm;
using namespace std;

char Cxt001Pass::ID = 0;
VectorMemoryClass vectorMemoryClass;

//With the next variable we trace the free value.
//The second element of the tuple contains a program variable pointer
//while the first contain a temporary value, which represent a pogram
//vaeriable bitcased
tuple< Value *,Value *,CallInst *> auxFree = tuple<Value *,Value *,CallInst *>(NULL,NULL,NULL); 

void Cxt001Pass::getAnalysisUsage(AnalysisUsage &AU) const {
  // Specifies that the pass will not invalidate any analysis already built on the IR
  AU.setPreservesAll();
  // Specifies that the pass will use the analysis LoopInfo
  AU.addRequiredTransitive<LoopInfoWrapperPass>();
}


///Extract the original free parameter.
void extractFreeValueFromLoad(Value * val,TargetLibraryInfo &targetLibraryInfo){
	if (const CallInst * call = dyn_cast<CallInst>(val) ){
		if ((call->getArgOperand(0))->hasName())
			vectorMemoryClass.insertFree(call,call->getArgOperand(0),targetLibraryInfo);
		if (call->getArgOperand(0) == std::get<0>(auxFree)){
			vectorMemoryClass.insertFree(call,std::get<1>(auxFree),targetLibraryInfo);
		}
	}
}

///Extract the variable which is asigned to a malloc/new call
Value * extractMallocValueFromStore(Instruction &I){
	if (StoreInst * storeInst = dyn_cast<StoreInst>(&I)){
		Value * operand0 = storeInst->getOperand(0);
		Value * operand1 = storeInst->getOperand(1);
		if ( vectorMemoryClass.insertMallocProgramVariable(operand0,operand1) )
		{
			string a = operand1->getName();	
		}
	}
}

/// Get the variable which store malloc pointer result
/// Then we will mathc it with the next store instrucction because this
/// does not get the real variable. Just a temporary one
void extractMallocValue(Value *val,TargetLibraryInfo targetLibraryInfo){
	Value * aux = NULL;
	if (const CallInst * call = dyn_cast<CallInst>(val) ){
		aux = static_cast<Value *>(val);
		// Test if malloc need cast the returned pointer to the original variable, which 
		// this pointer might be assigned. 
		for (Value::const_user_iterator begin = call->user_begin(), end = call->user_end();
			begin != end;){
				if (const BitCastInst *bitCastInst = dyn_cast<BitCastInst>(*begin++)) {
					aux = static_cast<Value *>(const_cast<BitCastInst *>(bitCastInst));
				}
			}
		vectorMemoryClass.insertMalloc(call,aux,targetLibraryInfo);
	}
}

///Check if a value corresponds to a malloc/new or free/delete call.
///If so, we store the result.
///It update the size requested by mallocs in a function;
void countMemoryFunctions(Value &val,FunctionInfo &info, TargetLibraryInfo &targetLibraryInfo, const DataLayout &dataLayout){
	if ( isMallocLikeFn( &val,&targetLibraryInfo,false ) ){
		if (CallInst * call = dyn_cast<CallInst>(&val) ){
			Value * constant = call->getOperand(0);
			if ( ConstantInt * cons= dyn_cast <ConstantInt>(constant) ){
				info.mem.size+=cons->getSExtValue();
			}
		}
		extractMallocValue(&val,targetLibraryInfo);
	}
	if ( isFreeCall ( &val,&targetLibraryInfo ) ){
		extractFreeValueFromLoad(&val,targetLibraryInfo);
	}
	 	
}

///Iterate over the function's instructions
///to get useful information
bool Cxt001Pass::runOnFunction(Function &F) {
	TargetLibraryInfoImpl targetLibraryInfoImpl = TargetLibraryInfoImpl(triple);
	TargetLibraryInfo targetLibraryInfo = TargetLibraryInfo(targetLibraryInfoImpl);
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
					if ( Value *value = dyn_cast<Value>(&I) )
						countMemoryFunctions(*value,functionInfo,targetLibraryInfo,module->getDataLayout());
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
	vectorMemoryClass.debug();
  return false;
}

void Cxt001Pass::printTotals(){
	int tops=0;
	int fops=0;
	int bytesTotales;
	int i=0;
	cout << "Datos por funcion" << "\n";
	for ( FunctionInfo f : functionOperationsVector ){
		i++;
		cout << "Function: " << f.getName() << "\n";
		cout << "Numero total de operaciones: " << f.getFunOps() << "\n";
		cout << "Número total de operaciones en punto flotante: " << f.f.ftotals << "\n";
		cout << "Bytes reservados por la función: " << f.mem.size << "\n";
		bytesTotales+=f.mem.size;
		tops+=f.getFunOps();
		fops+=f.f.ftotals;
	}

	cout << "\n" <<  "Datos globales: " << "\n";
	cout << "Reserva de memoria total: " << bytesTotales << "\n";
	cout << "Número de instrucciones totales: " << tops << "\n";
	cout << "Número de instrucciones en punto flotante: " << fops << "\n";
	cout << "Media de instrucciones por función: " << tops/i << "\n";
	cout << "Media de instrucciones en punto flotante por instrucción: " << fops/i << "\n";
}
///Prints the useful class values
void Cxt001Pass::print(raw_ostream &O, const Module *M) const {
  O << "For module: " << M->getName() << "\n";
}
