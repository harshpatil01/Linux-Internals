#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666);
    char *shm_ptr = (char *)shmat(shmid, NULL, 0);

    // Read the string from shared memory
    printf("Reading from shared memory: %s\n", shm_ptr);

    // Reverse the string
    int length = strlen(shm_ptr) - 1;
    for (int i = 0; i < length / 2; ++i) {
        char temp = shm_ptr[i];
        shm_ptr[i] = shm_ptr[length - i - 1];
        shm_ptr[length - i - 1] = temp;
    }

    printf("Writing to Shared memory ...\n");

    // Detach shared memory
    shmdt(shm_ptr);

    return 0;
}
