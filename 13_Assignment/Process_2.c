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
    int pipefds[2], shmid, read_count;
    char *shm;
    char *path = "/tmp/myfifo";
    char buff[SIZE];

    key = 'A';

    if ((shmid = shmget(key, SMSIZE, 0666)) == -1) {
        perror("shmget");
        return 1;
    }

    if ((pipe(pipefds)) == -1) {
        perror("pipe");
        return 1;
    }

    printf("SHMID is %d\n", shmid);
    pid = fork();

    if (pid > 0) {
        int status;
        close(pipefds[0]);

        printf("Reversing the string...\n");

        shm = shmat(shmid, NULL, 0);

        for (int i = 0; i < strlen(shm); i++) {
            buff[i] = shm[strlen(shm) - i - 1];
        }

        buff[strlen(shm)] = '\0';

        printf("After reversing the string: %s\n", buff);

        write(pipefds[1], buff, sizeof(buff));
        shmdt(shm);

        wait(&status);
    } else if (pid == 0) {
        close(pipefds[1]);

        char buff[SIZE];
        int fd;

        read(pipefds[0], buff, sizeof(buff));

        if ((mkfifo(path, 0666)) == -1) {
            perror("mkfifo");
            return 1;
        }

        printf("Writing the reverse string into the fifo special file\n");

        fd = open(path, O_WRONLY);

        if (fd == -1) {
            perror("open");
            return 1;
        }

        write(fd, buff, sizeof(buff));
        close(fd);
    } else {
        perror("Child Process Operation is not successful\n");
        return 1;
    }

    return 0;
}
