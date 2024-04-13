#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "account.h"
/* Save account information */
int main(int argc, char *argv[])
{
   FILE *fp;
   int id, balance;
   char name[24];
   int cur_balance, money;
   char answer;
   struct account *record, *head = NULL, *tail = NULL, *temp = NULL;

   if (argc < 2) {
      fprintf(stderr, "사용법 : %s file\n", argv[0]);
      exit(1);
   }
   if ((fp = fopen(argv[1],"wb")) == -1) {
      perror(argv[1]);
      exit(2);
   }

   printf("%-9s %-8s %-4s (ID=-1 to set the account information)\n","ID", "Name", "Balance");
   while(scanf("%d %s %d", &id, name ,&balance)==3){
	   if(id == -1) break;
	   record = (struct account*)malloc(sizeof(struct account));
	   if(record == NULL){
		   perror("malloc");
		   exit(3);
	   }

	   record->id = id;
	   strcpy(record->name ,name);
	   record->balance = balance;

	   if(head == NULL){
	  		   head = record;
	  		   tail = record;
	  	   }else{
	  		   tail->next = record;
	  		   tail = record;
	  	   }
   }

   record = head;
   temp = record;

   printf("Account Information \n");
   while(1){
	   printf("ID:%8d\t Name:%4s\t Balance:%4d\n", record->id, record->name, record->balance);
	   printf("Press N or W (N: Next account, W: withdraw money): ");
	   scanf("%s", &answer);
	   if(answer == 'W'){
		   printf("Amount of money to withdraw: ");
		   scanf("%d", &money);
		   cur_balance = record->balance - money;
		   if (cur_balance < 0){
    		   printf("should be smaller than %d\n", record->balance);
    		   do{
    			   printf("Amount of money to withdraw: ");
    			   scanf("%d", &money);
    			   cur_balance = record->balance - money;
    		     }while(cur_balance < 0);
    		   }
    		   record->balance = cur_balance;
    		   continue;
    	  }
	   if(record->next == NULL) record = temp;
	   else record = record->next;
      }
   fclose(fp);
   exit(0);
}
