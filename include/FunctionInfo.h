/* This headers contains the declaration of FunctionInfo class, which stores
 * the necessary information of a particular function:
 *	-Number of mallocs and frees
 *	-Number of float points operations ( fadd,fsub,fdiv,...)
 *	-Unique Id for each function
 *	-Function name
 * It also provides a method to print class data.
*/
#include <iostream>
#include "VectorMemoryClass.h"
using namespace std;

struct Memoria {
	int mallocs;
	int frees;
	size_t size;
	Memoria () : mallocs(0), frees(0), size(0) {};
}; 

struct Flotantes {
	int fadd;
	int fmul;
	int fsub;
	int fdiv;
	int frem;
	int fcmp;
	int ftotals;
	Flotantes(): fadd(0),fmul(0),fsub(0),
	fdiv(0),frem(0),fcmp(0),ftotals(0) {};

};

class FunctionInfo {
	private:
		int funId = 0; //Function ID
		string name;  //Function's name
		unsigned funOps = 0; //KPI_1: Operations per function counter
	public:
		VectorMemoryClass vectorMemoryClass;
		Flotantes f;
		Memoria mem;	
		void setFunId(int val){
			funId = val;
		}
		void setName(string val){
			name = val;
		}
		void setFunOps(int val){
			funOps = val;
		}
		int getFunId(){ return funId;}
		string getName(){return name;}
		unsigned getFunOps(){return funOps;}
		void increaseFunOps(){funOps++;}
		
};
