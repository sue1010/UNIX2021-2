#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"

int main(int argc, char* argv[])
{
    int fd, id, input;
    char c;
    struct account record;
    if (argc < 2) {
        fprintf(stderr, "사용법 : %s file\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror(argv[1]);
        exit(2);
    }
    do {
        printf("Account ID: ");
        if (scanf("%d", &id) == 1) {
            lseek(fd, (long)(id - START_ID) * sizeof(record), SEEK_SET);
            if ((read(fd, &record, sizeof(record)) > 0) && (record.id != 0)) {
                printf("ID:%8d\t Name:%4s\t Balance:%4d\n",
                    record.id, record.name, record.balance);
                printf("Amount of money to withdraw: ");
                scanf("%d", &input);
                if (input < 0) {
                    record.balance -= input;
                    lseek(fd, (long)-sizeof(record), SEEK_CUR);
                    write(fd, &record, sizeof(record));
                }
                else {
                    if (input <= record.balance) {
                        record.balance -= input;
                        lseek(fd, (long)-sizeof(record), SEEK_CUR);
                        write(fd, &record, sizeof(record));
                    }
                    else
                        printf("should be smaller than %d\n", record.balance);
                }

                printf("Balance: %d\n", record.balance);

            }
            else printf("레코드 %d 없음\n", id);
        }
        else printf("입력오류\n");
        printf("계속하겠습니까?(Y/N)");
        scanf(" %c", &c);
    } while (c == 'Y');
    printf("end\n");
    close(fd);
    exit(0);
}

