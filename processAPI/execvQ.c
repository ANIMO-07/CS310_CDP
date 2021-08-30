#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>   
#include <sys/wait.h>
int main() 
{ 
	char* ar[]={"/bin/ls","/home/sriramk/",NULL};
	
	for (int i = 0; i < 10; i++)
	{ 
		printf("%d\n", i); 
		if (i == 2) 
			execv("ls", ar); 
	} 
	return 0;
}
