#include "Cxt001Pass.h"
#include "llvm/IR/Value.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Analysis/MemoryBuiltins.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "VariableMap.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Function.h"
#include <sys/ioctl.h>
#include <tuple>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>

using namespace llvm;
using namespace std;
//The next variable store correspondences between temporary and not temporary variables
VariableMap varmap;
char Cxt001Pass::ID = 0;

void Cxt001Pass::getAnalysisUsage(AnalysisUsage &AU) const {
  // Specifies that the pass will not invalidate any analysis already built on the IR
  AU.setPreservesAll();
  // Specifies that the pass will use the analysis LoopInfo
  AU.addRequiredTransitive<LoopInfoWrapperPass>();
}

/**
This procedure store the last instruction inside a Call.
*/
void bitcastMallocValue(Value *val,TargetLibraryInfo targetLibraryInfo,FunctionInfo &info){
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
		info.vectorMemoryClass.insertMalloc(call,aux,targetLibraryInfo);
	}
}

/**Check if a Value is a call to malloc/free or new/delete function.
If the callInstance is stored, with its correspondent Variable on
the class which contains the function information.
*/
void countMemoryFunctions(Value &val,FunctionInfo &info, TargetLibraryInfo &targetLibraryInfo, const DataLayout &dataLayout){
	if ( isMallocLikeFn( &val,&targetLibraryInfo,false ) ){
		bitcastMallocValue(&val,targetLibraryInfo,info);
		if (CallInst * call = dyn_cast<CallInst>(&val) ){
			Value * constant = call->getOperand(0);
			if ( ConstantInt * cons= dyn_cast <ConstantInt>(constant) ){
				info.mem.size+=cons->getSExtValue();
			}
		}
	}
	if ( isFreeCall (&val, &targetLibraryInfo) ){
		if ( CallInst * call = dyn_cast<CallInst>(&val) ) {
			info.vectorMemoryClass.insertFree(call, varmap.getOriginalValue(call->getArgOperand(0)), targetLibraryInfo);
		}
	}
}

/**Analyze functions called by other functions looking for mallocs/free or news/deletes 
related with the caller function.
Note that we need pass the class which contains the function information to add the 
neccessary information to it.
*/
void computeFunction(Value * val,FunctionInfo &functionInfo, const DataLayout &dataLayout, TargetLibraryInfo &targetLibraryInfo){
	if ( CallInst * call = dyn_cast<CallInst>(val) ){
		Function * function = call->getCalledFunction();
		int i= 0;
		for (Function::arg_iterator begin=function->arg_begin(), end=function->arg_end();begin != end;){
			if (  Value * aux = dyn_cast<Value>(begin) ){
				varmap.setPair(call->getOperand(i), aux);
			}
			i++;
			begin++;
		}
		unsigned op;
		for ( BasicBlock &BB : *function){
			for ( Instruction &I : BB ){
				op=I.getOpcode();
				switch(op){
					case Instruction::Call:
						if ( Value *value = dyn_cast<Value>(&I) ){
						  countMemoryFunctions(*value,functionInfo,targetLibraryInfo,dataLayout);
							 computeFunction(value, functionInfo,dataLayout,targetLibraryInfo);
						}
					break;
				case Instruction::BitCast:
					if ( BitCastInst * bitCastInst = dyn_cast<BitCastInst>(&I) ){
						if ( Value * aux = varmap.getOriginalValue(bitCastInst->getOperand(0)))
							varmap.setPair(aux, bitCastInst);
						else
							varmap.setPair(bitCastInst->getOperand(0), bitCastInst);
					}
					
				break;
				case Instruction::Load:
					if ( LoadInst * loadInst = dyn_cast<LoadInst>(&I) ){
						if ( Value * aux = varmap.getOriginalValue(loadInst->getOperand(0)))
							varmap.setPair(aux,loadInst);
						else
							varmap.setPair(loadInst->getOperand(0), loadInst);
					}
				break;
				case Instruction::Store:
					if ( StoreInst * storeInst = dyn_cast<StoreInst>(&I) ){
						if ( storeInst->getOperand(0)->hasName() ){
							if (Value * aux = varmap.getOriginalValue(storeInst->getOperand(0)))
								varmap.setPair(aux,storeInst->getOperand(1));
							else
								varmap.setPair(storeInst->getOperand(0),storeInst->getOperand(1));
						}
						else
							varmap.setPair(storeInst->getOperand(1),storeInst->getOperand(0));
					}
					default:
					break; 
				}
			}
		}
	}
}

/**Iterate over the function's instructions
to get useful information*/
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
					if ( Value *value = dyn_cast<Value>(&I) ){
						countMemoryFunctions(*value,functionInfo,targetLibraryInfo,module->getDataLayout());
						computeFunction(value, functionInfo,module->getDataLayout(),targetLibraryInfo);
					}
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
					if ( BitCastInst * bitCastInst = dyn_cast<BitCastInst>(&I) ){
						varmap.setPair(bitCastInst->getOperand(0), bitCastInst);
					}
					
				break;
				case Instruction::Load:
					if ( LoadInst * loadInst = dyn_cast<LoadInst>(&I) ){
						varmap.setPair(loadInst->getOperand(0), loadInst);
					}
				break;
				case Instruction::Store:
					if ( StoreInst * storeInst = dyn_cast<StoreInst>(&I) ){
						if ( storeInst->getOperand(0)->hasName() )
							varmap.setPair(storeInst->getOperand(0),storeInst->getOperand(1));
						else
							varmap.setPair(storeInst->getOperand(1),storeInst->getOperand(0));
					}
				default:
				break; 
		} 
		functionInfo.increaseFunOps(); //KPI_1: Operations per function counter
    }
  }
  functionOperationsVector.push_back(functionInfo); //Insert in funOpVector 
	for ( pair<Value *, Value *> par : varmap.variablesMap ){
		functionInfo.vectorMemoryClass.insertMallocProgramVariable(par.first,par.second);
	}
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
		f.vectorMemoryClass.debug();
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
void printStripe(int winsize){//Prints a line of hyphen characters (-)
	int i;
	if (winsize==62 or winsize==68 or winsize==65) cout << "-";
	if (winsize<62 or winsize==64) cout << "--";
	for (i = 0; i < winsize-2; i++){
		cout << "-";
		}
	cout << "\n";
	}
void printHeaderElement(string element, int totalspace){ //Prints formated header element
	int len = element.size();
	int firstspace = (totalspace-len)/2;
	int secondspace = totalspace-len-firstspace;
	if(len >= totalspace) {
		firstspace = 0; secondspace = 0;
	}
	cout << left << std::setw(firstspace) << std::setfill(' ') << "|";
	cout << element;
	cout << right << std::setw(secondspace) << std::setfill(' ') << "|";
	}
void printFirstHeader(){  //Prints header for the first table. (Function report)
	struct winsize size;
	int winsize, space, surplusspace;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size); //Gets window size from terminal in linux OS
	winsize = size.ws_col; //70 aprox is fine
	space = winsize/4;
	surplusspace = winsize % 4;
	cout << "Function Report: \n";
	printStripe(winsize);
	printHeaderElement("Nombre", space);
	printHeaderElement("Nº Ops", space);
	printHeaderElement("Bytes reservados", space+surplusspace);
	printHeaderElement("Nº Float Ops", space);
	cout << endl;
	printStripe(winsize);
	cout << endl;
}


///Prints the useful class values
void Cxt001Pass::print(raw_ostream &O, const Module *M) const {
	string modname = M->getName();
	cout << "For module: " << modname << "\n";
	printFirstHeader();
    //printFirstTable(); Pendiente
    //printSecondHeader(); Pendiente
    //printSecondTable(); Pendiente
    //printGlobalInformation(); PARA ESTO IGUAL SIRVE printTotals()
    
}
