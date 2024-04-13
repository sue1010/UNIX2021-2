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
		int sfd, result, clientfd, k, k2;
		int ac_num, i, idx, money, cur_balance;
		char index[MAXLINE], output[MAXLINE], outmsg[MAXLINE], outmsg2[MAXLINE];

	   struct sockaddr_un serverAddr;

	   clientfd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
	   serverAddr.sun_family = AF_UNIX;
	   strcpy(serverAddr.sun_path, "convert");

	   do{
		   result = connect(clientfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
		   if(result == -1)sleep(1);
	   }while(result == -1);

	   printf("Insert account index: ");
	   fgets(index, MAXLINE, stdin);
	   write(clientfd, index, MAXLINE);

	   sleep(1);
	   k = read(clientfd, outmsg, MAXLINE);
	   printf("%s", outmsg);

	   if(access("convert",0) != 0){
	   		   exit(0);
	   	   }

	   sleep(1);
	   printf("set the amount of money to withdraw: ");
	   fgets(output, MAXLINE, stdin);
	   write(clientfd, output, MAXLINE);

	   sleep(1);
	   k2 = read(clientfd, outmsg2, MAXLINE);
	   printf("%s", outmsg2);
	   close(clientfd);
	   exit(0);
}

