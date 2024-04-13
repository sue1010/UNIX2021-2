#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "account.h"

int main(int argc, char *argv[])
{
	int fd, file, i, length, length2;
	char index[1];
	char out[3];

	i = 2;

	do{
		fd = open("myPipe", O_WRONLY);
		if(fd == -1)sleep(1);
	}while(fd==-1);

	while(i!=0){
		if(i==2){
			printf("Insert account index: ");
			scanf("%s", index);
			length = strlen(index)+1;
			write(fd, index, length);
		}else{
			if(access("myPipe", 0) != 0){	//access("myPipe", 0) != 0;	//fd = open("myPipe", O_WRONLY)
				perror("error");
				exit(1);
			}
			printf("set the account of money to withdraw: ");
			scanf("%s", out);
			length2 = strlen(out)+1;
			write(fd, out, length2);
		}
		i--;
	}
	close(fd);
	exit(0);
	return 0;
}
