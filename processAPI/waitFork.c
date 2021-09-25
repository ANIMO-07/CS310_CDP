#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	pid_t pids[10];
	int i;
	printf("PARENT pid=%d\n",getpid());
	for (i = 9; i >= 0; --i) {
		pids[i] = fork();
		if (pids[i] == 0) {
			sleep((i+1)*3);
			printf("CHILD exiting %d\n",getpid());
			_exit(0);
		}
	}

	for (i = 0; i <= 9; ++i)
		waitpid(pids[i], NULL, 0);
		printf("CHILD exiting %d\n",pids[i]);
		printf("PARENT exiting\n");

	return 0;
}


// Child exiting 21047
// Child exiting 21046
// Child exiting 21045
// .
// .
// .

