// command to disable ASLR: echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

#include<stdio.h>
#include<unistd.h>
int staticVar=0; //a static variable

void printMe() {
	printf("FunctionPrintMe\n");
	
}
int main(){
	int localVar;
	localVar =20;
	staticVar+=1;
	printf("Static Variable: Address:%x;Value:%d\n",&staticVar,staticVar);
	printf("Local Variable: Address:%x;Value:%d\n",&localVar,localVar);
	return 0;
}

