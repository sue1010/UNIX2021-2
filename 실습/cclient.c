#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "locker.h"
#define MAX 100
#define DEFAULT_PROTOCOL 0

struct locker *locker;

int main(){
	int clientfd, result;
	char num[MAX];//how many locker
	char select[MAX];//what number select locker
	char buf[MAX];
	char SetPassword[4];
	char choice[1];
	struct sockaddr_un serverAddr, clientAddr;

	clientfd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path, "state");

	do{
		result = connect(clientfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
		if(result == -1) sleep(1);
	}while(result==-1);

	printf("set number of locker: ");	// number of locker
	scanf("%s", num);
	write(clientfd, num, strlen(num)+1);

	while(1){
		read(clientfd, buf, strlen(buf)+1);
		printf("%s", buf);
		buf[MAX] = "";
		for (int i = 0; i < atoi(num); i++){
			read(clientfd, buf, strlen(buf) + 1);
			printf("%s", buf);
			buf[MAX] = "";
		}

		printf("\nselect locker: ");
		scanf("%s", select);
		write(clientfd, select, MAX);

		sleep(1);
		read(clientfd, buf, strlen(buf)+1);
		printf("%s", buf);
		buf[MAX] = "";

		scanf("%s", SetPassword);
		write(clientfd, SetPassword, strlen(SetPassword)+1);

		sleep(1);
		read(clientfd, buf, strlen(buf)+1);
		printf("%s", buf);
		buf[MAX] = "";

		scanf("%s", choice);
		write(clientfd, choice, 1);
	}

}

