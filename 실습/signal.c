#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "account.h"

void alarmHandler(int signo);
void intHandler(int signo);

int sec = 20;
int pid2, pid3, status3;

int main(int argc, char *argv[]){
	int pid1, status1, status2;

	pid1 = fork();	//ac_create
	signal(SIGINT,intHandler);
	if(pid1 == 0){
		execlp("./ac_create", " ","stdb1",NULL);
		fprintf(stderr, "1st fail");
		exit(1);
	}
	waitpid(pid1, &status1, 0);

	alarm(5);
	signal(SIGALRM, alarmHandler);
	fflush(stdout);

	pid2 = fork();	//ac_update
	if (pid2 == 0) {
		execlp("./ac_update"," ", "stdb1",NULL);
		fprintf(stderr, "2nd fail");
		exit(2);
	}
	waitpid(pid2, &status2, 0);

}

void alarmHandler(int signo){
	sec -= 5;
	printf("\n%d sec\n", sec);

	if(sec==0){
		kill(pid2,SIGINT);
		printf("\n20sec alarm\n");
		printf("start ac_query\n");

		pid3 = fork();
		if(pid3==0){
			execlp("./ac_query"," ","stdb1", NULL);	// ac_query
			fprintf(stderr, "3rd fail");
			exit(3);
		}
		waitpid(pid3, &status3, 0);
	}
	alarm(5);
	signal(SIGALRM,alarmHandler);
}

void intHandler(int signo){
	printf("start ac_update\n");
	printf("Transer money in 20 seconds\n");
}
