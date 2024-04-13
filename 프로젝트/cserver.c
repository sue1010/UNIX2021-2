#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "clocker.h"
#define MAX 1000
#define DEFAULT_PROTOCOL 0

void SetLocker(int count);
void PrintLocker(int number, int cfd);
struct locker* locker;

int main() {
    int sfd, cfd, clientlen;
    char num[MAX]; // set number of locker;
    char select[MAX];   //select locker
    int select_int, count = 0; //count = error count;
    char setpassword[4];
    char sig[10], choice[10];   // sig -> locker empty or not
    char memo[500];
    int Locknum;
    int plen;

    struct sockaddr_un serverAddr, clientAddr;

    clientlen = sizeof(clientAddr);

    sfd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, "state");
    unlink("state");
    bind(sfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    listen(sfd, 1);

    while (1) {
        cfd = accept(sfd, (struct sockaddr*)&clientAddr, &clientlen);
        printf("set number of locker: ");
        scanf("%s", num);
        Locknum = atoi(num);

        locker = (struct locker*)malloc(Locknum * sizeof(struct locker));
        SetLocker(Locknum);

        write(cfd, num, MAX);
        while (1) {
            count = 0;
            PrintLocker(Locknum, cfd);
            //select
            read(cfd, select, MAX);
            select_int = atoi(select) - 1;

            if (strcmp(locker[select_int].now, "empty") == 0) {
                sprintf(sig, "o");   //o->empty
                write(cfd, sig, MAX);
                read(cfd, setpassword, MAX);

                strcpy(locker[select_int].password, setpassword);
                strcpy(locker[select_int].now, "occupied");

                read(cfd, memo, 500);
                strcpy(locker[select_int].memo, memo);

            }
            else if (strcmp(locker[select_int].now, "error") == 0) {
                sprintf(sig, "error");
                write(cfd, sig, MAX);
            }
            else {
                sprintf(sig, "f");
                write(cfd, sig, MAX);

                while (1) {
                    read(cfd, setpassword, MAX);
                    if (strcmp(locker[select_int].password, setpassword) == 0) {
                        sprintf(sig, "t");
                        write(cfd, sig, MAX);
                        read(cfd, choice, MAX);
                        if (strcmp(choice, "Y") == 0) {
                            strcpy(locker[select_int].password, "0000");
                            strcpy(locker[select_int].now, "empty");
                            strcpy(locker[select_int].memo, "");
                        }
                        else {
                            sprintf(memo, "%s", locker[select_int].memo);
                            write(cfd, memo, 500);
                            read(cfd, memo, 500);
                            strcpy(locker[select_int].memo, memo);
                        }
                        break;
                    }
                    else {
                        count++;
                        if (count == 5) {
                            sprintf(sig, "error");
                            write(cfd, sig, MAX);
                            strcpy(locker[select_int].password, "----");
                            strcpy(locker[select_int].now, "error");
                            break;
                        }
                        else {
                            sprintf(sig, "f");
                            printf("not correct: %s", sig);
                            write(cfd, sig, MAX);
                        }
                    }
                }
            }
        }
    }
}


void SetLocker(int count) {
    for (int i = 0; i < count; i++) {
        locker[i].index = i + 1;
        strcpy(locker[i].password, "0000");
        strcpy(locker[i].now, "empty");
        strcpy(locker[i].memo, "");
    }
}

void PrintLocker(int number, int cfd) {
    char buf[MAX] = "";
    for (int j = 0; j < number; j++) {
        sprintf(buf, "locker [%d] | %s \n", locker[j].index, locker[j].now);
        write(cfd, buf, MAX);
    }
}
