



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 5

struct ThreadArgs {
    int* array;
    int start;
    int end;
};

int global_sum = 0;
int global_max = 0;
pthread_mutex_t mutex;

void* calculateSum(void* args) {
    struct ThreadArgs* threadArgs = (struct ThreadArgs*)args;
    int local_sum = 0;

    for (int i = threadArgs->start; i <= threadArgs->end; ++i) {
        local_sum += threadArgs->array[i];
    }

    pthread_mutex_lock(&mutex);
    global_sum += local_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* findMax(void* args) {
    struct ThreadArgs* threadArgs = (struct ThreadArgs*)args;
    int local_max = threadArgs->array[threadArgs->start];

    for (int i = threadArgs->start + 1; i <= threadArgs->end; ++i) {
        if (threadArgs->array[i] > local_max) {
            local_max = threadArgs->array[i];
        }
    }

    pthread_mutex_lock(&mutex);
    if (local_max > global_max) {
        global_max = local_max;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    int size;
    printf("Enter the size of array: ");
    scanf("%d", &size);

    if (size < MAX_THREADS) {
        printf("Error: Array size should be at least %d\n", MAX_THREADS);
        return 1;
    }

    int* array = (int*)malloc(size * sizeof(int));

    printf("Enter the array elements:\n");
    for (int i = 0; i < size; ++i) {
        scanf("%d", &array[i]);
    }

    pthread_t sum_threads[MAX_THREADS];
    pthread_t max_threads[MAX_THREADS];
    struct ThreadArgs args[MAX_THREADS];

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }

    int chunkSize = size / MAX_THREADS;
    int remainder = size % MAX_THREADS;

    for (int i = 0; i < MAX_THREADS; ++i) {
        args[i].array = array;
        args[i].start = i * chunkSize;
        args[i].end = (i + 1) * chunkSize - 1;
        if (i == MAX_THREADS - 1) {
            args[i].end += remainder;
        }

        pthread_create(&sum_threads[i], NULL, calculateSum, (void*)&args[i]);
        pthread_create(&max_threads[i], NULL, findMax, (void*)&args[i]);
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(sum_threads[i], NULL);
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(max_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Max = %d Sum = %d\n", global_max, global_sum);

    free(array);

    return 0;
}
