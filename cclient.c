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

struct locker* locker;

int main() {
    int clientfd, result;
    int n;   // number of locker
    int plen;
    char num[MAX];   // number of locker
    char buf[MAX];   // read buf
    char select[MAX]; // select locker
    int select_int;
    char setmsg[MAX], msg[MAX];
    char setpassword[MAX];
    char check[10], sig[10], choice[10];   // sig -> locker empty or not
    char memo[500], writing[500];
    struct sockaddr_un serverAddr, clientAddr;

    clientfd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, "state");

    do {
        result = connect(clientfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (result == -1) sleep(1);
    } while (result == -1);

    read(clientfd, num, MAX);
    n = atoi(num);
    while (1) {
        printf("---------[current locker state]---------\n");
        for (int i = 0; i < n; i++) {
            read(clientfd, buf, MAX);
            printf("%s", buf);
        }
        //select
        printf("select locker: ");
        scanf("%s", select);
        write(clientfd, select, atoi(select));
        select_int = atoi(select) - 1;

        read(clientfd, sig, MAX);

        if (strcmp(sig, "o") == 0) {
            while (1) {
                printf("set password: ");
                scanf("%s", setpassword);
                plen = strlen(setpassword);
                if ((strlen(setpassword) > plen) || (strlen(setpassword) < plen)) {
                    printf("!! You must enter %d digits !!\n", plen);
                }
                else {
                    printf("set!\n");
                    write(clientfd, setpassword, MAX);

                    printf("=====================================\n");
                    printf("========== write something ==========\n");
                    printf("write >>> ");
                    scanf("%s", memo);
                    write(clientfd, memo, 500);

                    break;
                }
            }

        }
        else if (strcmp(sig, "error") == 0) {
            printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("!!You can't access this locker!!\n");
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
        }
        else {
            do {
                printf("insert password(%d digits): ", plen);
                scanf("%s", setpassword);
                write(clientfd, setpassword, MAX);
                read(clientfd, sig, MAX);
                if (strcmp(sig, "t") == 0) break;
                if ((strlen(setpassword) > plen) || (strlen(setpassword) < plen))
                    printf("!! You must enter %d digits !!\n", plen);
                else if (strlen(setpassword) == plen)
                    printf("!! Enter password again !!\n!! You must enter %d digits !!\n", plen);

            } while (strcmp(sig, "f") == 0);

            if (strcmp(sig, "error") != 0) {
                printf("Do you want to unlock this locker? (Y/N) ");
                scanf("%s", choice);
                write(clientfd, choice, MAX);
                if (strcmp(choice, "N") == 0) {
                    printf("\n========== write something =============\n");
                    printf("================= memo =================\n");
                    read(clientfd, memo, 500);
                    printf("%s\n", memo);
                    scanf("%s", writing);
                    write(clientfd, writing, 500);
                }
            }
        }
    }
}
