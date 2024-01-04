


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 3

struct ThreadArgs {
    unsigned long int start;
    unsigned long int end;
    unsigned long int result;
};

void *calculateFactorial(void *args) {
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)args;
    threadArgs->result = 1;

    for (unsigned long int i = threadArgs->start; i <= threadArgs->end; ++i) {
        threadArgs->result *= i;
    }

    return NULL;
}

int main() {
    unsigned long int number;
    printf("Enter the number: ");
    scanf("%lu", &number);

    pthread_t threads[MAX_THREADS];
    struct ThreadArgs threadArgs[MAX_THREADS];

    unsigned long int chunkSize = number / MAX_THREADS;
    unsigned long int remainder = number % MAX_THREADS;

    for (int i = 0; i < MAX_THREADS; ++i) {
        threadArgs[i].start = i * chunkSize + 1;
        threadArgs[i].end = (i + 1) * chunkSize;
        if (i == MAX_THREADS - 1) {
            threadArgs[i].end += remainder;
        }
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_create(&threads[i], NULL, calculateFactorial, (void *)&threadArgs[i]);
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    unsigned long int finalResult = 1;
    for (int i = 0; i < MAX_THREADS; ++i) {
        finalResult *= threadArgs[i].result;
    }

    printf("Factorial of %lu is %lu\n", number, finalResult);

    return 0;
}
