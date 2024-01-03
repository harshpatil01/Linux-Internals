#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SIZE 20
#define SMSIZE 30

int main() {
    pid_t pid;
    key_t key;
    int pipefd[2], shmid, option, fd;
    char str[SIZE], o;
    char *shm;
    char *path = "/tmp/myfifo";
    char buff[SIZE];

    key = 'A';

    if ((shmid = shmget(key, SMSIZE, IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        return 1;
    }

    if ((pipe(pipefd)) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid > 0) {
        close(pipefd[0]);

        do {
            printf("Enter Your Choice\n");
            printf("\t1.Write\n\t2.Read\n");
            scanf("%d", &option);

            switch (option) {
                case 1:
                    printf("Enter the string\n");
                    scanf("%s", str);
                    printf("You wrote: %s\n", str);

                    printf("Writing the string into the pipe\n");
                    write(pipefd[1], str, sizeof(str));

                    wait(NULL);
                    break;

                case 2:
                    printf("Reading the string after its completed operation from fifo special file.......\n");
                    fd = open(path, O_RDONLY);

                    if (fd == -1) {
                        perror("open");
                        return 1;
                    }

                    read(fd, buff, sizeof(buff));
                    close(fd);

                    printf("After the completed operation the string is \"%s\"\n", buff);
                    break;
            }

            printf("Do you want to continue?\n");
            getchar();
            scanf("\n%c", &o);

            if (o != 'Y' && o != 'y') {
                break;
            }
        } while (1);
    } else if (pid == 0) {
        close(pipefd[1]);
        char buff[SIZE], temp[SIZE];
        int i, len;
        
        read(pipefd[0], buff, sizeof(buff));

        len = strlen(buff);

        for (i = 0; i < len; i++) {
            if (buff[i] >= 'a' && buff[i] <= 'z') {
                temp[i] = buff[i] - 32;
            } else {
                temp[i] = buff[i];
            }
        }

        temp[i] = '\0';

        printf("After the operation the string is: %s\n", temp);
        printf("Writing the string into the shared memory......\n");

        shm = shmat(shmid, NULL, 0);
        strcpy(shm, temp);
        shmdt(shm);
    } else {
        perror("Child Process Operation is not successful\n");
        return 1;
    }

    return 0;
}
