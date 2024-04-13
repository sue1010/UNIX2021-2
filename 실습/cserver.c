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

void SetLocker(int count);
void PrintLocker(int n, int cfd);
struct locker *locker;

int main(){
	int sfd, cfd, clientlen; // socket
	int selectnum, Locknum;
	int choicenum;
	int length, checknum;
	char num[MAX], Set[MAX], choice[MAX], select[MAX], buf[MAX];
	struct sockaddr_un serverAddr, clientAddr;	// socket

	signal(SIGCHLD, SIG_IGN);
	clientlen = sizeof(clientAddr);

	// SOCKET
	sfd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path, "state");
	unlink("state");
	bind(sfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	listen(sfd, 1);

	while(1){
		cfd = accept(sfd, (struct sockaddr *)&clientAddr, &clientlen);
		//set locker
		read(cfd, num, strlen(num)+1);
		Locknum = atoi(num);
		locker = (struct locker*)malloc(Locknum*sizeof(struct locker));

		SetLocker(Locknum);

		while(1){
			PrintLocker(Locknum, cfd);
			read(cfd, select, MAX);
			selectnum = atoi(select);
			if(locker[selectnum].password == NULL){
				buf[MAX] = "";
				sprintf(buf, "set password(four digits): ");
				write(cfd, buf, strlen(buf)+1);
				sleep(1);
				read(cfd, Set, strlen(Set)+1);
				locker[selectnum].password = Set;
				locker[selectnum].now = 'occupied';
				write(cfd, "Set", 3);
			}
			else{
				do{
					buf[MAX] = "";
					sprintf(buf, "insert password(four digits): ");
					write(cfd, buf, strlen(buf)+1);
					sleep(1);
					read(cfd, Set, strlen(Set)+1);
				}while(locker[selectnum].password != Set);

				buf[MAX] = "";
				sprintf(buf, "Do you want to unlock this locker? (Y/N) ");
				write(cfd, buf, strlen(buf)+1);

				read(cfd, choice, strlen(choice)+1);
				if(choice == 'Y'){
					locker[selectnum].now = 'empty';
					locker[selectnum].password = '0000';
				}
			}
		}
	}

}

void SetLocker(int count){
	for(int j = 0; j < count; j++){
		locker[j].index = j+1;
		locker[j].password = '0000';
		//locker[j].now = "empty";
	}
}

void PrintLocker(int n, int cfd){
	char buf[MAX];
	// print current state
	sprintf(buf, "current locker state: \n");
	write(cfd, buf, strlen(buf)+1);
	buf[MAX] = "";
	for(int i = 0; i < n; i++){
		sprintf(buf, "Locker: %d| %d ", locker[i].index, locker[i].now);

	}
	write(cfd, buf, strlen(buf)+1);
}

