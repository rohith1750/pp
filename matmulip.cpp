#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to allocate memory for a 2D matrix
int** allocate_matrix(int rows, int cols) {
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }
    return matrix;
}

// Function to free memory allocated for a 2D matrix
void free_matrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to perform matrix multiplication
void matrix_multiply(int **mat1, int **mat2, int **result, int rows1, int cols1, int cols2) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// Function to perform matrix addition
void matrix_addition(int **mat1, int **mat2, int **result, int rows, int cols) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

int main() {
    int rows, cols, num_threads;
    char operation;

    // Input matrix dimensions and number of threads
    printf("Enter dimensions of the matrices (rows columns): ");
    scanf("%d %d", &rows, &cols);
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    // Check if the number of threads is valid
   

    // Allocate memory for matrices
    int **matrix1 = allocate_matrix(rows, cols);
    int **matrix2 = allocate_matrix(rows, cols);
    int **result = allocate_matrix(rows, cols);

    // Input elements of matrices
    printf("Enter elements of the first matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix1[i][j]);
        }
    }
    printf("Enter elements of the second matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix2[i][j]);
        }
    }

    // Input operation choice
    printf("Enter operation choice (m for multiplication, a for addition): ");
    scanf(" %c", &operation);

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Perform matrix operation based on user choice
    double start_time = omp_get_wtime();
    if (operation == 'm') {
        matrix_multiply(matrix1, matrix2, result, rows, cols, cols);
    } else if (operation == 'a') {
        matrix_addition(matrix1, matrix2, result, rows, cols);
    } else {
        printf("Invalid operation choice. Exiting.\n");
        return 1;
    }
    double end_time = omp_get_wtime();

    // Display the result
    printf("Resultant matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    // Display execution time
    printf("Execution Time: %.6f seconds\n", end_time - start_time);

    // Free dynamically allocated memory
    free_matrix(matrix1, rows);
    free_matrix(matrix2, rows);
    free_matrix(result, rows);

    return 0;
}

