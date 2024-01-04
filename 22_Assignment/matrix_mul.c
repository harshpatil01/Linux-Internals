


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Structure to pass arguments to threads
typedef struct thread_data {
    short m1_row;
    short m1_col;
    short m2_col;
    int **matrix1;
    int **matrix2;
    int **result;
    short cur_row;
} Thread_data_t;

// Function to allocate memory for a matrix
int **allocateMatrix(short rows, short cols) {
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (short i = 0; i < rows; ++i) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }
    return matrix;
}

// Function to deallocate memory for a matrix
void deallocateMatrix(int **matrix, short rows) {
    for (short i = 0; i < rows; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to perform matrix multiplication for a single row
void *multiplyRow(void *args) {
    Thread_data_t *thread_data = (Thread_data_t *)args;

    for (short i = 0; i < thread_data->m2_col; ++i) {
        thread_data->result[thread_data->cur_row][i] = 0;
        for (short j = 0; j < thread_data->m1_col; ++j) {
            thread_data->result[thread_data->cur_row][i] +=
                thread_data->matrix1[thread_data->cur_row][j] * thread_data->matrix2[j][i];
        }
    }

    pthread_exit(NULL);
}

int main() {
    short m1_rows, m1_cols, m2_rows, m2_cols;

    // Input for matrix dimensions
    printf("Enter M1 rows and columns: ");
    scanf("%hd %hd", &m1_rows, &m1_cols);

    printf("Enter M2 rows and columns: ");
    scanf("%hd %hd", &m2_rows, &m2_cols);

    if (m1_cols != m2_rows) {
        printf("Error: M1 columns should be equal to M2 rows.\n");
        return 1;
    }

    // Allocate matrices
    int **matrix1 = allocateMatrix(m1_rows, m1_cols);
    int **matrix2 = allocateMatrix(m2_rows, m2_cols);
    int **result = allocateMatrix(m1_rows, m2_cols);

    // Input for matrix values
    printf("Enter M1 values:\n");
    for (short i = 0; i < m1_rows; ++i) {
        for (short j = 0; j < m1_cols; ++j) {
            scanf("%d", &matrix1[i][j]);
        }
    }

    printf("Enter M2 values:\n");
    for (short i = 0; i < m2_rows; ++i) {
        for (short j = 0; j < m2_cols; ++j) {
            scanf("%d", &matrix2[i][j]);
        }
    }

    // Create thread data array
    Thread_data_t *thread_data = (Thread_data_t *)malloc(m1_rows * sizeof(Thread_data_t));
    pthread_t *threads = (pthread_t *)malloc(m1_rows * sizeof(pthread_t));

    // Initialize and create threads
    for (short i = 0; i < m1_rows; ++i) {
        thread_data[i].m1_row = m1_rows;
        thread_data[i].m1_col = m1_cols;
        thread_data[i].m2_col = m2_cols;
        thread_data[i].matrix1 = matrix1;
        thread_data[i].matrix2 = matrix2;
        thread_data[i].result = result;
        thread_data[i].cur_row = i;

        pthread_create(&threads[i], NULL, multiplyRow, (void *)&thread_data[i]);
    }

    // Join threads
    for (short i = 0; i < m1_rows; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Print result
    printf("Result is:\n");
    for (short i = 0; i < m1_rows; ++i) {
        for (short j = 0; j < m2_cols; ++j) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    // Deallocate memory
    deallocateMatrix(matrix1, m1_rows);
    deallocateMatrix(matrix2, m2_rows);
    deallocateMatrix(result, m1_rows);
    free(thread_data);
    free(threads);

    return 0;
}
