#include <stdio.h>
#include <stdlib.h>


int print_matrix(int matrix_size, double** matrix, int step) {
    int i;
    int j;
    if (!matrix) {
        return -1;
    }
    printf("\nStep %d. Matrix:\n", step);
    for (i = 0; i < matrix_size; i++) {
        for (j = 0; j < matrix_size; j++) {
            printf("%.2lf ", matrix[i][j]);
        }
        printf("\n");
    }
    return 1;
}


int Gaussian_method(int matrix_size, double** matrix, double* determinant) {
    if (!matrix) {
        return -1;
    }
    int i;
    int j;
    int k;
    double factor;
    *determinant = 1;
    for (i = 0; i < matrix_size - 1 && matrix[i][i] != 0; i++) {
        for (j = i + 1; j < matrix_size; j++) {
            factor = matrix[j][i] / matrix[i][i];
            for (k = i; k < matrix_size; k++) {
                matrix[j][k] -= matrix[i][k] * factor;
            }
        }
        print_matrix(matrix_size, matrix, i+1);
    }
    if (matrix[i][i] == 0) {
        *determinant = 0;
    } else {
        for (i = 0; i < matrix_size; i++) {
            *determinant *= matrix[i][i];
        }
    }
    return 1;
}


int print_info() {
    printf("==========================\n");
    printf("Gaussian_method in C 1.0.1\n");
    printf("==========================\n");
    return 1;
}


int main(void) {
    print_info();
    printf("Enter the size of matrix:\n");
    int matrix_size;
    int i;
    int j;
    double determinant;
    scanf("%d", &matrix_size);
    double** matrix = calloc(matrix_size, sizeof(double*));
    for (i = 0; i < matrix_size; i++) {
        matrix[i] = calloc(matrix_size, sizeof(double));
    }
    printf("Enter the matrix:\n");
    for (i = 0; i < matrix_size; i++) {
        for (j = 0; j < matrix_size; j++) {
            scanf("%lf", matrix[i] + j);
        }
    }
    printf("\nOriginal matrix:\n");
    for (i = 0; i < matrix_size; i++) {
        for (j = 0; j < matrix_size; j++) {
            printf("%.2lf ", matrix[i][j]);
        }
        printf("\n");
    }
    Gaussian_method(matrix_size, matrix, &determinant);
    printf("\nDeterminant of matrix: %0.2lf", determinant);
    free(matrix);
    return 0;
}