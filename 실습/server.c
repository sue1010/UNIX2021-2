#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "account.h"

int main(int argc, char *argv[])
{
	int fd;
   int ac_num, i, idx, money, cur_balance, n,length, length2;
   char index[1];
   char out[3];
   struct account *record;

   unlink("myPipe");
   mkfifo("myPipe",0660);

   fd = open("myPipe",O_RDONLY);
   printf("set the number of account: ");
   scanf("%d", &ac_num);

   record = (struct account *)malloc(ac_num * sizeof(struct account));

   printf("Index   %-9s %-8s %-4s\n", "ID", "Name", "Balance");
   for (i=0; i < ac_num; i++)
   {
	   printf("   %d   ", i+1);
	   scanf("%d %s %d", &record[i].id, record[i].name, &record[i].balance);
   }
   printf("wait..\n");
   for(int k = 0; k<1;k++){
	   if(k==0){
		   n = read(fd,index,length);
		   idx = atoi(index);
		   if (idx <= 0 || idx > ac_num)
		   {
			   printf("Index should be in 1 to %d\n", ac_num);
			   unlink("myPipe");
			   break;
		   }
		   else
			   printf("Index: %2d ID:%8d\t Name:%4s\t Balance:%4d\n", idx, record[idx-1].id, record[idx-1].name, record[idx-1].balance);
	   }
		   n = read(fd,out,length2);
		   money = atoi(out);
		   cur_balance = record[idx-1].balance - money;
		   	if (cur_balance < 0)
		   		printf("should be smaller than %d\n", record[idx-1].balance);
		   	else
		   	{
		   		record[idx-1].balance = cur_balance;
		   		printf("Balance: %d\n", record[idx-1].balance);
		   	}

   }
   free(record);
   exit(0);
}
