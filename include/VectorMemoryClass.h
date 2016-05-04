#include <vector>
#include <iostream>
#include "MemoryClass.h"
using namespace std;
using namespace llvm;

class VectorMemoryClass {
	public:
		VectorMemoryClass() {
			memoryClassVector = new vector<MemoryClass *>();
			notVariableMallocs = new vector<MemoryClass *>();
		};



		bool insertMalloc(const CallInst * call, const Value * val, TargetLibraryInfo &targetLibraryInfo ){
			if ( !val->hasName() ){
				MemoryClass * m = new MemoryClass();
				m->setMalloc(call,targetLibraryInfo);
				m->setValue(val);
				
				notVariableMallocs->push_back(m);
				return true;
			}
			for ( MemoryClass * memoryClass : *memoryClassVector ){
				if ( memoryClass->getValue() == val )
					if ( memoryClass->setMalloc(call,targetLibraryInfo) )
						return true;
			}
			MemoryClass * aux = new MemoryClass();
			if (aux->setMalloc(call,targetLibraryInfo)){
				if ( aux->setValue(val) ){
					memoryClassVector->push_back(aux);
					return true;
				}
			}
			return false;
		}

		bool insertFree( const CallInst * call,const Value * val, TargetLibraryInfo &targetLibraryInfo ){
			for ( MemoryClass * memoryClass : *memoryClassVector ){
				if ( memoryClass->getValue() == val )
					if ( memoryClass->setFree(call,targetLibraryInfo) )
						return true;
			}
			MemoryClass * aux = new MemoryClass();
			
			if (aux->setFree(call,targetLibraryInfo))
				if ( aux->setValue(val) ){
					memoryClassVector->push_back(aux);
					return true;
				}
			return false;
		}
		const vector<MemoryClass *> * getVector(){
			return memoryClassVector;
		}
	
	bool insertMallocProgramVariable(const Value * bitcasted, const Value * original){
		for (MemoryClass * m : *notVariableMallocs){
			const Value * aux = m->getValue();
			if (aux == NULL)
				continue;
			if ( aux == original ){
				return true;
			}
			if ( aux == bitcasted ){
				if ( MemoryClass * mOriginal = getMemoryClassFromValue(memoryClassVector,original) ){
					mOriginal->malloc = m->getMalloc(); 
					return true;
				}else {
					MemoryClass * auxMen = new MemoryClass();
					auxMen->malloc = m->getMalloc();
					auxMen->free = NULL;
					auxMen->value = original;
					memoryClassVector->push_back(auxMen);
					return true;
				}
			}
		}
	
		return false;
	}
	
	void debug(){
		for (MemoryClass * m : *memoryClassVector){
			const CallInst * malloc = m->getMalloc();
			const CallInst * free = m->getFree();
			const Value * val = m->getValue();
			string a;
			if ( val != NULL )
				a = val->getName();
			if ( malloc == NULL )
				cout << "No malloc for variable: " << a << "\n";
			else if ( free == NULL )
				cout << "Not free for Variable: " << a << "\n";
		/*
			else if ( m->getFreeFirst() )
				cout << "Free is first for variable: " << a << "\n";
*/
			else 
				cout << "Malloc and free for variable: " << a << "\n";
		}
	}
	
	private:
		static MemoryClass * getMemoryClassFromValue(vector<MemoryClass *> * vector,const Value * val){
			for (MemoryClass * m : *vector){
				if (m->getValue() == val )
					return m;
			}
			return NULL;
		}
		vector<MemoryClass *> * memoryClassVector;
		vector<MemoryClass *> * notVariableMallocs;
};
