#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "C5.h"

int main(){
    double *A, *B, *C;
    int i,j,n;
    char sgn;

    system("chcp 1251");
    system("cls");

    printf("Введите размер матрицы:\n");
    scanf("%d", &n);


    A = (double*)malloc(n*n*sizeof(double));

    B = (double*)malloc(n*n*sizeof(double));

    for (i = 0; i < n; i++ ){
        for (j = 0; j < n; j++){
            printf("Введите значения матрицы A[%d][%d] = ", i,j);
            scanf("%lf", (A + i*n + j));
        }
    }

    for (i = 0; i < n; i++ ){
        for (j = 0; j < n; j++){
            printf("Введите значения матрицы B[%d][%d] = ", i,j);
            scanf("%lf", (B + i*n + j));
        }
    }
    printf("Введите + - * (сумма вычитание или умножение матриц %c \n)", sgn);
    scanf(" %c", &sgn);
    C = calculate(A, B, sgn, n);
    
    for (i=0;i<n;i++){
        for (j=0;j<n;j++){
            printf("%5lf", *(A + i*n + j));
        }
        printf("\n");
    }
    free(A); free(B); free(C);
    return 0;
}