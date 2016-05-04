#include "llvm/IR/Value.h"
using namespace std;
using namespace llvm;
class VariableMap {
	public:
		Value * getOriginalValue( Value * temp ) {
			return variablesMap[temp];
		}
		bool setPair ( Value * real, Value * temp){
			if ( real->hasName() ){
				variablesMap[temp]=real;	
				return true;
			}else{
				if ( Value * aux = variablesMap[real] ){
					variablesMap[temp]=aux;
					return true;
				}
			}
			return false;
		}
		map<Value * , Value *> variablesMap;
};
