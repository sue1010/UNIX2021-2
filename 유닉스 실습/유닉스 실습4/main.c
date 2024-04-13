#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"
/* Save account information */
int main(int argc, char *argv[])
{
   FILE *fp;
   int n;
   int id;
   int cur_balance, money;
   char c;
   struct account *record;

   if (argc < 2) {
      fprintf(stderr, "사용법 : %s file\n", argv[0]);
      exit(1);
   }
   if ((fp = fopen(argv[1],"wb")) == -1) {
      perror(argv[1]);
      exit(2);
   }

   printf("set the number of account: ");
   scanf("%d", &n);
   if(n<=0){
	   fprintf(stderr, "error");
	   fprintf(stderr, "end");
	   exit(3);
   }

   printf("%-4s %-9s %-8s %-4s\n","Index" ,"ID", "Name", "Balance");
   record = (struct accont*)malloc(n*sizeof(struct account));
   for(int i = 0; i < n; i++){
	   if(record == NULL){
		   perror("malloc");
	   	   exit(4);
	   	  }
	   printf("%d ", i+1);
	   scanf("%d %s %d", &record[i].id, record[i].name, &record[i].balance);
	   if(record[i].id - START_ID < 0){
		   printf("should be larger than %d", START_ID);
	   }
   }
   while(1) {
      printf("Account index to access: ");
      if (scanf("%d", &id) == 1) {
    	  if(id > n){
    		  printf("Index should be in 1 to %d\n", n);
    		  do{
    			  printf("Account index to access: ");
    			  scanf("%d", &id);
    		  }while(id > n);
    	  }
    	  printf("Index: %4d ID:%8d\t Name:%4s\t Balance:%4d\n", id,record[id-1].id, record[id-1].name, record[id-1].balance);
    	  while(1){
    		  printf("Amount of money to withdraw: ");
    		  scanf("%d", &money);
    		  cur_balance = record[id-1].balance - money;
    		  if (cur_balance < 0){
    		     printf("should be smaller than %d\n", record[id-1].balance);
    		     do{
    		     	  	printf("Amount of money to withdraw: ");
    		     	    scanf("%d", &money);
    		     	    cur_balance = record[id-1].balance - money;
    		     }while(cur_balance < 0);
    		   }
    		   record[id-1].balance = cur_balance;
    		   printf("Balance: %d\n", record[id-1].balance);
    	  }
      } else printf("입력오류\n");
   }
   fclose(fp);
   exit(0);

}
