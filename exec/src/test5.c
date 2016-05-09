#include <stdlib.h>
void free1( int * free1_arg_1 ){
	free ( free1_arg_1 );
}

void res1( int * res1_arg_1 ){
	int * res1_1 = (int *) malloc ( sizeof (int) ); 
	free (res1_1);
	res1_arg_1 = (int *) malloc ( sizeof (int) );
}

int main(void){
	int * main_1;
	res1 (main_1);
	free1 (main_1);
}
