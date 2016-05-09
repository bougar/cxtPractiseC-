#include <stdlib.h>
void free1( int * free1_arg_1 ){
	free ( free1_arg_1 );
}

void res1( int * res1_arg_1 ){
	res1_arg_1 = (int *) malloc ( sizeof (int) );
}

void inter( int * inter_arg_1 ){
	res1 ( inter_arg_1 );
}

void inter2( int * inter2_arg_1 ){
	free1(inter2_arg_1);
}

int main(void){
	int * main_1;
	inter (main_1);
	inter2 (main_1);
}
