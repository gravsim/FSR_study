#include <stdio.h>
#include <stdlib.h>




int Gaussian_method(int matrix_size, int**matrix) {
    

}


int main(void) {
    int matrix_size;
    int i;

    scanf("%d", &matrix_size);
    int** matrix = calloc(matrix_size, sizeof(int*));
    for (i = 0; i < matrix_size; i++) {
        scanf("%d", matrix[i] + j);
    }
    Gaussian_method(matrix_size, matrix);
    free(matrix);
    return 0;
}