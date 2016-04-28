#include "llvm/IR/Value.h"
#include "llvm/Support/Casting.h"
#include "llvm/Analysis/MemoryBuiltins.h"

class MemoryClass {
	public:
		MemoryClass() : malloc(NULL),free(NULL),value(NULL),freeFirst(false) {};
				
		const llvm::CallInst * setMalloc(llvm::CallInst * call,llvm::TargetLibraryInfo &targetLibraryInfo){
			if (llvm::Value * val = llvm::dyn_cast<llvm::Value>(call))
				if ( llvm::isMallocLikeFn (val, &targetLibraryInfo, false ) ){
					malloc = call;
					return malloc;	
				}
			return NULL;
		}

		const llvm::CallInst * setFree(llvm::CallInst * call,const llvm::TargetLibraryInfo &targetLibraryInfo){
			if (llvm::Value * val = llvm::dyn_cast<llvm::Value>(call))
				if ( llvm::isFreeCall (val, &targetLibraryInfo ) ){
					if ( !malloc )
						freeFirst = true;
					malloc = call;
					return malloc;	
				}
			return NULL;
		}
		const llvm::Value * setValue ( llvm::Value * val ){
			if (val->hasName()){
				value = val;
				return value;
			}
			return NULL;
		}

		const llvm::Value * getValue(){
			return value;
		}
		
		const llvm::CallInst * getMalloc(){
			return malloc;
		}

		const llvm::CallInst * getFree(){
			return free;
		}
	
	private:
		llvm::CallInst * malloc;
		llvm::CallInst * free;
		llvm::Value * value;
		bool freeFirst;
	
};
