#include <stdio.h>

int main(int argc, char* argv[]){
	printf("Number of args = %d\n",argc);
	printf("Hello World!\n");
	for( int i = 0;i<argc;i++){
		printf("%s ", argv[i]);
	}
	printf("\n");
	return 1;

}
