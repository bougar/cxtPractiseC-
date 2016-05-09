#include <stdlib.h>

int main(void){
	int * main_nofree =(int *) malloc ( sizeof (int) );
	int * main_nomalloc;
	int * main_1 = (int *) malloc (sizeof (int) );
	free (main_nomalloc);
	free (main_1);
}
