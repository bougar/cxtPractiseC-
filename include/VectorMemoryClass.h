#include <vector>
#include "MemoryClass.h"
using namespace std;
using namespace llvm;

class VectorMemoryClass {
	public:
		bool insertMalloc( CallInst * call, Value * val, TargetLibraryInfo &targetLibraryInfo ){
			for ( MemoryClass * memoryClass : memoryClassVector ){
				if ( memoryClass->getValue() == val )
					if ( memoryClass->setMalloc(call,targetLibraryInfo) )
						return true;
					return false;
			}
			MemoryClass * aux = new MemoryClass();
			
			if (aux->setMalloc(call,targetLibraryInfo))
				if ( aux->setValue(val) ){
					memoryClassVector.push_back(aux);
					return true;
				}
			return false;
		}

		bool insertFree( CallInst * call, Value * val, TargetLibraryInfo &targetLibraryInfo ){
			for ( MemoryClass * memoryClass : memoryClassVector ){
				if ( memoryClass->getValue() == val )
					if ( memoryClass->setFree(call,targetLibraryInfo) )
						return true;
					return false;
			}
			MemoryClass * aux = new MemoryClass();
			
			if (aux->setFree(call,targetLibraryInfo))
				if ( aux->setValue(val) ){
					memoryClassVector.push_back(aux);
					return true;
				}
			return false;
		}
		const vector<MemoryClass *> * getVector(){
			return &memoryClassVector;
		}

	private:
		vector<MemoryClass *> memoryClassVector;
};
