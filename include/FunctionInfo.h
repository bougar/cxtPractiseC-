/* This headers contains the declaration of FunctionInfo class, which stores
 * the necessary information of a particular function:
 *	-Number of mallocs and frees
 *	-Number of float points operations ( fadd,fsub,fdiv,...)
 *	-Unique Id for each function
 *	-Function name
 * It also provides a method to print class data.
*/
#include <iostream>
using namespace std;

struct Memoria {
	int mallocs;
	int frees;
}; 

struct Flotantes {
	int fadd=0;
	int fmul=0;
	int fsub=0;
	int fdiv=0;
	int frem=0;
	int fcmp=0;
	int ftotals=0;
};

class FunctionInfo {
	private:
		int funId = 0;
		string name; 
		unsigned funOps = 0; //KPI_1: Operations per function counter
	public:
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
		string get_name(string val){return name;}
		unsigned getFunOps(int val){return funOps;}
		void increaseFunOps(){funOps++;}
	/*
	void print(raw_ostream &O){
			O << "\tNº: " << funId;
			O << "  Function: " << name << "\n";
			O << "\tOperations: " << funOps << "\n\n";
	}*/
	
};
