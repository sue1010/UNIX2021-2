#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include"account.h"
#define DEFAULT_PROTOCOL 0
#define MAXLINE 100


int main(){
		int sfd, cfd, clientlen, n, n2;
		int ac_num, i, idx, money, cur_balance;
		//int length1, length2;
		char index[MAXLINE], output[MAXLINE], msg[MAXLINE];

	   struct account *record;
	   struct sockaddr_un serverAddr, clientAddr;

	   signal(SIGCHLD, SIG_IGN);

	   sfd = socket(AF_UNIX,SOCK_STREAM,DEFAULT_PROTOCOL);
	   serverAddr.sun_family = AF_UNIX;
	   strcpy(serverAddr.sun_path, "convert");
	   unlink("convert");
	   bind(sfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	   listen(sfd, 5);



	   while(1){
		   cfd = accept(sfd,(struct sockaddr*)&clientAddr, &clientlen);
		   printf("set the number of account: ");
		   	   scanf("%d", &ac_num);

		   	   record = (struct account *)malloc(ac_num * sizeof(struct account));
		   	   printf("Index   %-9s %-8s %-4s\n", "ID", "Name", "Balance");
		   	   for (i=0; i < ac_num; i++)
		   	   {
		   		   printf("   %d   ", i+1);
		   		   scanf("%d %s %d", &record[i].id, record[i].name, &record[i].balance);
		   	   }
		   if(fork() == 0){
			   	  n = read(cfd, index, MAXLINE);
			   	  idx = atoi(index);
			   	  if (idx <= 0 || idx > ac_num)
				   {
					   sprintf(msg, "Error: Index should be in 1 to %d\n", ac_num);
					   write(cfd, msg, MAXLINE);
					   unlink("convert");
					   break;
				   }
			   	  sprintf(msg,"Index: %2d ID:%8d\t Name:%4s\t Balance:%4d\n", idx, record[idx-1].id, record[idx-1].name, record[idx-1].balance);
			   	  write(cfd, msg, MAXLINE);

			   	  sleep(1);
			   	  n2 = read(cfd, output, MAXLINE);
			   	  money = atoi(output);
			   	  cur_balance = record[idx-1].balance-money;
			   	  if (cur_balance < 0)
				   {
					   sprintf(msg, "should be smaller than %d\n", record[idx-1].balance);
				   }
			   	  else
				   {
					   record[idx-1].balance = cur_balance;
					   sprintf(msg, "Balance: %d\n", record[idx-1].balance);
				   }
			   	  write(cfd, msg, MAXLINE);
			   	  close(cfd);
			   	  //break;
			   	  exit(0);
		   }else close(cfd);
	   }
}







