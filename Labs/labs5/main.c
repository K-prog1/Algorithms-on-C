#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "C5.h"

int main(){
    double *A, *B, *C;
    int i,j,n;
    char sgn;

    printf("Введите размер матрицы:\n");
    scanf("%d", &n);


    A = malloc(n*n*sizeof(double));
    B = malloc(n*n*sizeof(double));

    printf("\nВведите матрицу A:\n");
    for (i = 0; i < n; i++ ){
        for (j = 0; j < n; j++){
            printf("A[%d][%d] = ", i,j);
            scanf("%d", (A + i*n + j));
        }
    }
    printf("\nВведите матрицу B:\n");
    for (i = 0; i < n; i++ ){
        for (j = 0; j < n; j++){
            printf(" B[%d][%d] = ", i,j);
            scanf("%d", (B + i*n + j));
        }
    }
    printf("Введите + - * (сумма вычитание или умножение матриц  \n)");
    scanf(" %c", &sgn);
    C = calculate(A, B, sgn, n);
    
    printf("\n Rezultat: \n");
    for (i=0;i<n;i++){
        for (j=0;j<n;j++){
            printf("%5d", C[i*n + j]);
        }
        printf("\n");
    }
    free(A); free(B); free(C);
    return 0;
}