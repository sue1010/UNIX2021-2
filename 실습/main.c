#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "account.h"

void childHandler(int signo);
int fd[2], pid;

int main(int argc, char *argv[])
{
   int ac_num, i, idx, money, cur_balance, n, length, length2;
   char index[1];
   char out[3];
   struct account *record;

   pipe(fd);

   signal(SIGCHLD, childHandler);
   pid = fork();
   if(pid==0){
	   close(fd[1]);
	   printf("[%d]Child process\n",getpid());
	   printf("set the number of account: ");
	   scanf("%d", &ac_num);

	   record = (struct account *)malloc(ac_num * sizeof(struct account));

	   printf("Index   %-9s %-8s %-4s\n", "ID", "Name", "Balance");
	   for (i=0; i < ac_num; i++)
	   {
	   	  printf("   %d   ", i+1);
	   	  scanf("%d %s %d", &record[i].id, record[i].name, &record[i].balance);
	    }

	   for(int k = 0; k<2; k++){

		   if(k == 0){
			   n = read(fd[0], index, length);
			   idx = atoi(index);
			   if (idx <= 0 || idx > ac_num)
			   {
				   printf("Index should be in 1 to %d\n", ac_num);
				   kill(pid, SIGINT);
			   }
			   else{
				   printf("Index: %2d ID:%8d\t Name:%4s\t Balance:%4d\n", idx, record[idx-1].id, record[idx-1].name, record[idx-1].balance);
			   }
		   }
		   else{
			   n = read(fd[0], out, length2);
			   money = atoi(out);
			   cur_balance = record[idx-1].balance - money;
			   if (cur_balance < 0)
			   {
				   printf("should be smaller than %d\n", record[idx-1].balance);
			   }else{
				   record[idx-1].balance = cur_balance;
				   printf("Balance: %d\n", record[idx-1].balance);
			   }
		   	 }
	   }

   }
   else{
	   close(fd[0]);
	   sleep(10);
	   printf("[%d]Parent process\n",getpid());
	   for(int j = 0; j<2; j++){
		   if(j == 0){
			   printf("Insert account index: ");
			   scanf("%s", index);
			   length = strlen(index)+1;
			   write(fd[1], index, length);
		   }else{
			   sleep(1);
			   printf("Amount of money to withdraw: ");
			   scanf("%s", out);
			   length2 = strlen(out)+1;
			   write(fd[1], out, length2);
			   sleep(1);
		   }
	   }
   }
   free(record);
   exit(0);
}

void childHandler(int signo){
	exit(0);
}

