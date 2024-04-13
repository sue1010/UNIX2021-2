#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	int pid1, pid2, pid3, child, status1, status2, status3;

	pid1 = fork();
	if(pid1 == 0){
		printf("[%d]\n", getpid());
		execvp("./ac_create", argv);
		fprintf(stderr, "1st fail");
		exit(1);
     }
	waitpid(pid1, &status1, 0);

	pid2 = fork();
	if (pid2 == 0) {
		printf("[%d]\n", getpid());
		execvp("./ac_update", argv);
		fprintf(stderr, "2nd fail");
		exit(2);

	 }
	waitpid(pid2, &status2, 0);

	pid3 = fork();
	if(pid3 == 0){
		printf("[%d]\n", getpid());
		execvp("./ac_query", argv);
		fprintf(stderr, "3rd fail");
		exit(3);
	}
	waitpid(pid3, &status3, 0);

}

