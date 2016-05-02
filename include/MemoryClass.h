#include "llvm/IR/Value.h"
#include <iostream>
#include "llvm/Support/Casting.h"
#include "llvm/Analysis/MemoryBuiltins.h"

class MemoryClass {
	public:
		MemoryClass() : malloc(NULL),free(NULL),value(NULL),freeFirst(false) {};
				
		const llvm::CallInst * setMalloc(const llvm::CallInst * call,llvm::TargetLibraryInfo &targetLibraryInfo){
			if (const llvm::Value * val = llvm::dyn_cast<llvm::Value>(call))
				if ( llvm::isMallocLikeFn (val, &targetLibraryInfo, false ) ){
					malloc = call;
					return malloc;	
				}
			return NULL;
		}

		const llvm::CallInst * setFree(const llvm::CallInst * call,const llvm::TargetLibraryInfo &targetLibraryInfo){
			if (const llvm::Value * val = llvm::dyn_cast<llvm::Value>(call))
				if ( llvm::isFreeCall (val, &targetLibraryInfo ) ){
					if ( !malloc )
						freeFirst = true;
					free = call;
					return free;	
				}
			return NULL;
		}

		const llvm::Value * setValue (const llvm::Value * val ){
			value = val;
			return value;
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

		bool getFreeFirst(){
			return freeFirst;
		}
	
	private:
		const llvm::CallInst * malloc;
		const llvm::CallInst * free;
		const llvm::Value * value;
		bool freeFirst;
		friend class VectorMemoryClass;
	
};
