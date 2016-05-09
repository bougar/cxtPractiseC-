class Test{
};
void free1( Test * free1_arg_1 ){
	delete free1_arg_1;
}
void res1( Test * res1_arg_1 ){
	res1_arg_1 = new Test();
}
int main(void){
	Test * main_1;
	res1 ( main_1 );
	free1 ( main_1 );
}
