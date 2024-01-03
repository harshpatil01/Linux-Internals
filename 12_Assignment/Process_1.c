#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <ctype.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    char *shm_ptr = (char *)shmat(shmid, NULL, 0);

    printf("Enter the string: ");
    fgets(shm_ptr, SHM_SIZE, stdin);

    // Convert string to uppercase
    for (int i = 0; i < SHM_SIZE; ++i) {
        if (shm_ptr[i] != '\0' && shm_ptr[i] != '\n') {
            shm_ptr[i] = toupper(shm_ptr[i]);
        } else {
            break;
        }
    }

    printf("Writing to shared memory ...... \n");

    // Wait for Process2 to finish reading
    sleep(2);

    // Read the reversed string from shared memory
    printf("Reading from shared memory: %s\n", shm_ptr);

    // Detach shared memory
    shmdt(shm_ptr);

    // Remove shared memory segment
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
